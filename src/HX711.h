/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
 * - Added Zero (as opposed to Tare)
 * - Made functions more in line with "Industry standard" weigher terms
 * SDM
 &
**/
#ifndef HX711_h
#define HX711_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HX711
{
	private:
		const byte PD_SCK;	    // Power Down and Serial Clock Input Pin
		const byte DOUT;		    // Serial Data Output Pin
		byte _gain;		    // amplification factor
		long _zero = 0;	    // used for zero weight
		long _tare = 0;	    // used for zero weight
		double _scale = 1;	// used to return weight in grams, kg, ounces, whatever

	public:

		// Initialize library with data output pin, clock input pin
		HX711(byte dout, byte pd_sck );

		virtual ~HX711();

		// Initialize library with gain factor.
		// Channel selection is made by passing the appropriate gain:
		// - With a gain factor of 64 or 128, channel A is selected
		// - With a gain factor of 32, channel B is selected
		// The library default is "128" (Channel A).
		void begin( byte gain = 128);

		// Check if HX711 is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready() const;

		// Wait for the HX711 to become ready
		void wait_ready(unsigned long delay_ms = 0) const;
		bool wait_ready_retry(int retries = 3, unsigned long delay_ms = 0) const;
		bool wait_ready_timeout(unsigned long timeout = 1000, unsigned long delay_ms = 0) const;

		// set the gain factor; takes effect only after a call to read()
		// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
		// depending on the parameter, the channel is also set to either A or B
		void set_gain(byte gain = 128);

		// waits for the chip to be ready and returns a reading
		long read() const;


		/// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
        /// Raw value is *multiplied* by this value [altered, was divisor; multiply is cheaper - SDM]
		void set_scale(float scale = 1.f);

		// get the current SCALE
		double get_scale() const;

		double scale(long raw) const { return raw*_scale; }
        
		// set _zero, the value that's subtracted from the actual reading (tare weight)
		void set_zero(long offset = 0);        

		// get the current _zero
		long get_zero() const;

		double get_tare() const;
		void set_tare(double t);

		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();
        
        
		// set the _zero value for abs weight; times = how many times to read the tare value
        void zero(byte times = 10);

		// set the _tare value for tare weight; times = how many times to read the tare value
		void tare(byte times = 10);
        
        long absolute(byte times = 10) const;
        long grossRaw(byte times = 10) const;
        double gross(byte times = 10) const;
        long nettRaw(byte times = 10) const;
        double nett(byte times = 10) const;
};

#endif /* HX711_h */
