/*
 * nRF_24.h
 *
 * Created: 07.02.2021 12:03:36
 *  Author: Zotov Egor
 */ 

#ifndef NRF_24_H_
	
	#define NRF_24_H_
	
	#define RF24_PORT PORTB
	#define RF24_DDR DDRB
	#define RF24_PIN PINB

	#define RF24_CSN DDB0
	#define RF24_CE DDB4
	#define RF24_IRQ DDB5
	
	#define RF24_ASSERT_CE RF24_PORT |= (1 << RF24_CE)
	#define RF24_DEASSERT_CE RF24_PORT &= ~(1 << RF24_CE)
	
	#define RF24_ASSERT_CSN RF24_PORT &= ~(1 << RF24_CSN)
	#define RF24_DEASSERT_CSN RF24_PORT |= (1 << RF24_CSN)
	
	#define RF24_PackageSize	32
	#define RF24_Channel		7
	#define Addr_Transmit		((uint8_t*)"TRSMT")
	#define Addr_Receive		((uint8_t*)"RECIV")
	
	extern void RF24_init(void);
	extern uint8_t RF24_ReadBuf(uint8_t cmd, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_WriteBuf(uint8_t cmd, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_ReadReg(uint8_t reg);
	extern uint8_t RF24_WriteReg(uint8_t reg, uint8_t val);
	extern uint8_t RF24_ReadRegBuf(uint8_t reg, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_WriteRegBuf(uint8_t reg, uint8_t* buf, uint8_t count);
	extern uint8_t RF24_GetPayloadLen();
	extern uint8_t RF24_Cmd(uint8_t cmd);
	extern uint8_t RF24_IsInterrupt(void);
	
	extern uint8_t RF24_Start(void);
	extern uint8_t RF24_Begin(uint8_t attempts);
	extern void RF24_Setup_Transmitter(void);
	extern void RF24_Setup_Receiver(void);
	extern void RF24_Send(uint8_t* data, uint8_t size);
	extern uint8_t RF24_Receive(uint8_t* buf, uint8_t size);
	
#endif /* NRF_24_H_ */