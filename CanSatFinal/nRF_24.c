/*
 * nRF_24.c
 *
 * Created: 07.02.2021 12:03:36
 *  Author: Zotov Egor
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"
#include "nRFdef.h"
#include "nRF_24.h"

extern void RF24_init() {
	RF24_DDR |= (1 << RF24_CSN) | (1 << RF24_CE);
	RF24_DDR &= ~(1 << RF24_IRQ);
	
	RF24_DEASSERT_CSN;
	RF24_DEASSERT_CE;
}
extern uint8_t RF24_ReadBuf(uint8_t cmd, uint8_t* buf, uint8_t count) {
	RF24_ASSERT_CSN;
	uint8_t status = spi_SendRecvByte(cmd);
	while (count--) { *(buf++) = spi_SendRecvByte(0xFF); }
	RF24_DEASSERT_CSN;
	return status;
}
extern uint8_t RF24_WriteBuf(uint8_t cmd, uint8_t* buf, uint8_t count) {
	RF24_ASSERT_CSN;
	uint8_t status = spi_SendRecvByte(cmd);
	while (count--) { spi_SendRecvByte(*(buf++)); }
	RF24_DEASSERT_CSN;
	return status;
}
extern uint8_t RF24_ReadReg(uint8_t reg) {
	RF24_ASSERT_CSN;
	spi_SendRecvByte((reg & 31) | R_REGISTER);
	uint8_t ans = spi_SendRecvByte(0xFF);
	RF24_DEASSERT_CSN;
	return ans;
}
extern uint8_t RF24_WriteReg(uint8_t reg, uint8_t val) {
	RF24_ASSERT_CSN;
	uint8_t status = spi_SendRecvByte((reg & 31) | W_REGISTER);
	spi_SendRecvByte(val);
	RF24_DEASSERT_CSN;
	return status;
}
extern uint8_t RF24_ReadRegBuf(uint8_t reg, uint8_t* buf, uint8_t count) {
	return RF24_ReadBuf((reg & 31) | R_REGISTER, buf, count);
}
extern uint8_t RF24_WriteRegBuf(uint8_t reg, uint8_t* buf, uint8_t count) {
	return RF24_WriteBuf((reg & 31) | W_REGISTER, buf, count);
}
extern uint8_t RF24_GetPayloadLen() {
	RF24_ASSERT_CSN;
	spi_SendRecvByte(R_RX_PL_WID);
	uint8_t ans = spi_SendRecvByte(0xFF);
	RF24_DEASSERT_CSN;
	return ans;
}
extern uint8_t RF24_Cmd(uint8_t cmd) {
	RF24_ASSERT_CSN;
	uint8_t status = spi_SendRecvByte(cmd);
	RF24_DEASSERT_CSN;
	return status;
}
extern uint8_t RF24_IsInterrupt() {
	return (RF24_PIN & RF24_IRQ) ? 0 : 1;
	//return !(RF24_ReadReg(FIFO_STATUS) & (1 << RX_EMPTY));
}
extern uint8_t RF24_Start() {
	RF24_DEASSERT_CE;
	for(uint8_t cnt = 100;;) {
		RF24_WriteReg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (0 << PRIM_RX)); // Выключение питания
		if (RF24_ReadReg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (0 << PRIM_RX))) { break; } //Проверка записанного регистра
		if (!cnt--) { return 0; } // Если после 100 попыток не удалось записать регистр, то выходим с ошибкой
		_delay_ms(1);
	}
	
	RF24_WriteReg(EN_AA, 0);
	RF24_WriteReg(EN_RXADDR, 0);
	RF24_WriteReg(SETUP_AW, SETUP_AW_5BYTES_ADDRESS);
	RF24_WriteReg(RF_CH, RF24_Channel);
	RF24_WriteReg(RF_SETUP, RF_SETUP_250KBPS | RF_SETUP_0DBM);
	
	RF24_WriteReg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (0 << PRIM_RX));
	return (RF24_ReadReg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (0 << PRIM_RX))) ? 1 : 0;
}
extern uint8_t RF24_Begin(uint8_t attempts) {
	RF24_init();
	while(attempts--) {
		if (RF24_Start()) { _delay_ms(2); RF24_ASSERT_CE; return 1; }
		_delay_ms(500);
	}
	return 0;
}
extern void RF24_Setup_Transmitter() {
	RF24_WriteRegBuf(TX_ADDR, &Addr_Transmit[0], 5);
}
extern void RF24_Setup_Receiver() {
	//RF24_WriteReg(EN_AA, (1 << ENAA_P0));
	RF24_WriteReg(EN_RXADDR, (1 << ERX_P0));
	RF24_WriteRegBuf(RX_ADDR_P0, &Addr_Transmit[0], 5); //&Addr_Receive[0]
	RF24_WriteReg(RX_PW_P0, RF24_PackageSize);
	
	RF24_WriteReg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX));
	RF24_WriteReg(STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
}
extern void RF24_Send(uint8_t* data, uint8_t size) {
	RF24_DEASSERT_CE;
	RF24_WriteReg(CONFIG, RF24_ReadReg(CONFIG) & ~((1 << PWR_UP) | (1 << PRIM_RX)));
	RF24_Cmd(FLUSH_TX);
	
	_delay_ms(10);
	RF24_WriteBuf(W_TX_PAYLOAD, data, size);
	RF24_WriteBuf(W_TX_PAYLOAD, data, size);
	
	_delay_ms(10);
	RF24_WriteReg(CONFIG, RF24_ReadReg(CONFIG) | (1 << PWR_UP));
	RF24_ASSERT_CE;
	
	_delay_ms(10);
	RF24_DEASSERT_CE;
	RF24_WriteReg(CONFIG, RF24_ReadReg(CONFIG) & ~(1 << PWR_UP));
}
extern uint8_t RF24_Receive(uint8_t* buf, uint8_t size) {
	if (!RF24_IsInterrupt()) { return 0; }
	
	RF24_DEASSERT_CE;
	uint8_t state = RF24_Cmd(NOP);
	RF24_WriteReg(STATUS, state);
	_delay_ms(10);
	
	uint8_t maxpackets = 4;
	if (((state & (7 << RX_P_NO)) != (7 << RX_P_NO)) && maxpackets--) {
		//if (RF24_GetPayloadLen() > 32) { RF24_Cmd(FLUSH_RX); state = RF24_Cmd(NOP); continue; }
		
		RF24_ReadBuf(R_RX_PAYLOAD, buf, size);
		RF24_WriteReg(STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
		RF24_WriteReg(CONFIG, RF24_ReadReg(CONFIG) | (1 << PWR_UP) | (1 << PRIM_RX));
		return 1;
	}
	
	RF24_ASSERT_CE;
	_delay_ms(10);
	return 0;
}