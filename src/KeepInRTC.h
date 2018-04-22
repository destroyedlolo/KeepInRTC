/* Keep data in ESP8266's memory
 *
 * 22/04/2018	First version
 */
#ifndef KEEPINRTC_H
#define KEEPINRTC_H	0.0100

class KeepInRTC {
	bool RTCvalid;		// Is stored memory valid ?
	uint32_t offset;	// Offset in memory for next data

	uint32_t key;		// To check if RTC is valid

public:
	KeepInRTC() : RTCvalid(false){
		if(ESP.rtcUserMemoryRead(0, (uint32_t *)&this->key, sizeof(this->key))){
			if( this->key == ESP.getFlashChipId() )
				this->RTCvalid = true;
		}

		if( !this->RTCvalid ){	// Initial values
			this->key = ESP.getFlashChipId();

			/* RTCvalid remains invalid in order to let other modules
			 * to initialise themselves to default values
			 */
			this->save();
		}

		this->offset = sizeof( this->key );
	}

	bool isValid( void ){ return RTCvalid; }

	void save( void ){
		ESP.rtcUserMemoryWrite(0, (uint32_t *)&this->key, sizeof(this->key));
	}

	uint32_t reserveData( uint32_t s ){
		uint32_t start = this->offset;
		this->offset += s;

		return( start );
	}

	class KeepMe {
		uint32_t *what;
		uint32_t size;
		uint32_t offset;

	public:
		KeepMe( KeepInRTC &ctx, uint32_t *w, uint32_t s ) : what(w), size(s) {
		/* ->	ctx : context managing the RTC memory
		 * 		w : which data to save
		 * 		s : size of the data to save
		 */
			this->offset = ctx.reserveData( s );

			if(ctx.isValid())	// previous data can be retrieved
				ESP.rtcUserMemoryRead(this->offset, this->what, this->size);
		}

		void save( void ){
			ESP.rtcUserMemoryWrite( this->offset, this->what, this->size );
		}
	};
};
#endif
