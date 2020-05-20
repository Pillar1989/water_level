#include <atmel_start.h>
#include <atomic.h>

#define CHAN_CNT  12

volatile uint8_t slave_address;
volatile uint8_t register_address = 200;
volatile uint8_t num_addresses;
volatile uint8_t num_reads;
volatile uint8_t num_writes;
volatile uint8_t num_stops;
volatile uint16_t touch_value = 0;

extern volatile uint8_t measurement_done_touch;
uint8_t level_status = 0;
uint8_t touch_result[2];

volatile uint8_t raw_value[CHAN_CNT];
int i;

void touch_status_display1(void)
{
	for (i = 0 ; i< CHAN_CNT;i++){
		level_status = 0;
		level_status = get_sensor_state(i) & KEY_TOUCHED_MASK;
		if (0u != level_status){
			raw_value[i] = get_sensor_node_signal(i);
		}
		else{
			raw_value[i] = 0;
		}
	}
	/*
	level_status = get_sensor_state(0) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		
		if(get_sensor_node_signal(0) > 200)
			touch_value |= 1 << 0;
		} else {
		
	}
	else{
		
	}
	level_status = get_sensor_state(1) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(1) > 200)
			touch_value |= 1 << 1 ;
		} else {
		
	}
	level_status = get_sensor_state(2) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(2) > 200)
			touch_value |= 1 << 2;
		} else {
		
	}
	level_status = get_sensor_state(3) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(3) > 200)
			touch_value |= 1 << 3;
		} else {
		
	}
	level_status = get_sensor_state(4) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(4) > 200)
			touch_value |= 1 << 4;
		} else {
		
	}
	level_status = get_sensor_state(5) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(5) > 200)
			touch_value |= 1 << 5;
		} else {
		
	}
	level_status = get_sensor_state(6) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(6) > 200)
			touch_value |= 1 << 6;
		} else {
		
	}
	level_status = get_sensor_state(7) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(7) > 200)
			touch_value |= 1 << 7;
		} else {
		
	}
	level_status = get_sensor_state(8) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(8) > 200)
			touch_value |= 1 << 8;
		} else {
		
	}
	level_status = get_sensor_state(9) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(9) > 200)
			touch_value |= 1 << 9;
		} else {
		
	}
	level_status = get_sensor_state(10) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(10) > 200)
			touch_value |= 1 << 10;
		} else {
		
	}
	level_status = get_sensor_state(11) & KEY_TOUCHED_MASK;
	if (0u != level_status) {
		if(get_sensor_node_signal(11) > 200)
			touch_value |= 1 << 11;
		} else {
		
	}
	touch_result[0] = (uint8_t)((uint16_t)touch_value>>8);
	touch_result[1] = (uint8_t)touch_value; 
	*/
}

void address_handler(){
	slave_address = I2C_0_read();
	I2C_0_send_ack();	// or send_nack() if we don't want to ack the address
	num_addresses++;
}

/*
uint8_t read_flag = 0;
void read_handler(){  // Master read operation
	if(0 == read_flag)
	{
		read_flag += 1;
		I2C_0_write(touch_result[0]);
	}
	else
	{
		read_flag = 0;
		I2C_0_write(touch_result[1]);	
	}
	//num_reads++;
	num_reads++;
}
*/
volatile uint8_t read_flag = 0;
void read_handler(){  
	if(read_flag >= CHAN_CNT){
		read_flag = 0;
	}
	I2C_0_write(raw_value[read_flag]);
	num_reads++;
}


void write_handler(){  // Master write handler
	register_address = I2C_0_read();
	I2C_0_send_ack();	// or send_nack() if we don't want to receive more data
	num_writes++;
}

void stop_handler(){
	num_stops++;
}

void error_handler(){
	while(1);
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	ENABLE_INTERRUPTS();

	I2C_0_set_read_callback(read_handler);
	I2C_0_set_write_callback(write_handler);
	I2C_0_set_address_callback(address_handler);
	I2C_0_set_stop_callback(stop_handler);
	I2C_0_set_collision_callback(error_handler);
	I2C_0_set_bus_error_callback(error_handler);
	I2C_0_open();
	
	cpu_irq_enable();
	/* Replace with your application code */
	while (1) {
		
		touch_process();
		if (measurement_done_touch == 1) {
			touch_value = 0;
			touch_status_display1();
		}
		for(int i = 0; i < 30000; i++){}
	}
}
