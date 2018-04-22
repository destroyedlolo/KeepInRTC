/* Store then retrieve some data from the RTC memory
 *
 *  This little piece of code is put in Public Domain.
 */

#include <KeepInRTC.h>

KeepInRTC kir;	// backup master

class Test : public KeepInRTC::KeepMe {
	struct {	// Data to be stored
		int cnt;
	} data;
public:
	Test() : KeepInRTC::KeepMe( kir, (uint32_t *)&this->data, sizeof(this->data) ){}

	void init( void ){
		this->data.cnt = 0;
		this->save();
		Serial.println("Init to 0");
	}

	void inc( void ){
		this->data.cnt++;
		this->save();
		Serial.print("\nIncremented to ");
		Serial.println( this->data.cnt );
	}
};

void setup( void ) {
	Serial.begin(115200);
	delay(100);
}

void loop( void ){
	Test tst;

	if( !kir.isValid() ){
		Serial.println("First run");
		tst.init();
	} else {
		tst.inc();
	}

	ESP.deepSleep( 5e6 );
}

