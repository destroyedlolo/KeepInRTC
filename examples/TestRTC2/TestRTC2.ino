/* Store then retrieve 2 data from the RTC memory
 * in order to demonstrate how to store multiple data
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

	void init( int val=0 ){
		this->data.cnt = val;
		this->save();
	}

	void inc( void ){
		this->data.cnt++;
		this->save();
	}

	int get( void ){
		return this->data.cnt;
	}
};

void setup( void ) {
	Serial.begin(115200);
	delay(100);
}

void loop( void ){
	Test tst, tst2;

	if( !kir.isValid() ){
		Serial.println("First run\nInitialise to default values");
		tst.init();
		tst2.init(2);
	} else {
		tst.inc();
		if( tst.get() == tst2.get() ){
			tst.init(0);
			tst2.inc();
		}
	}

	Serial.print("\ntst :");
	Serial.print( tst.get() );
	Serial.print(", tst2 :");
	Serial.println( tst2.get() );

	ESP.deepSleep( 5e6 );
}

