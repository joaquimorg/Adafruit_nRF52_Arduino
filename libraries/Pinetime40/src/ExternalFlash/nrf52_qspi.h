#ifndef NRF52_QSPI_H_
#define NRF52_QSPI_H_


void nrf52_qspi_begin(void);
void nrf52_qspi_end(void);
bool nrf52_qspi_run_command(uint8_t command);
bool nrf52_qspi_read_command(uint8_t command, uint8_t* response, uint32_t len);
bool nrf52_qspi_write_command(uint8_t command, uint8_t const* data, uint32_t len);
bool nrf52_qspi_erase_command(uint8_t command, uint32_t address);
bool nrf52_qspi_read_write_memory(bool read_op, uint32_t addr, uint8_t* data, uint32_t len);


#endif /* NRF52_QSPI_H_ */