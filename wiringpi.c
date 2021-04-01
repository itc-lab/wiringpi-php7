/* wiringpi extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_wiringpi.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static zend_class_entry *wiringpi_class_entry;

#define	WIRINGPI_CLASS_NAME		"WiringPi"

#define	NAMEandVALUE(val)	{ #val, val }
static const struct {
	const char* name;
	long value;
} constants[] = {

// wiringPi modes
	NAMEandVALUE( WPI_MODE_PINS ),
	NAMEandVALUE( WPI_MODE_GPIO ),
	NAMEandVALUE( WPI_MODE_GPIO_SYS ),
	NAMEandVALUE( WPI_MODE_PHYS ),
	NAMEandVALUE( WPI_MODE_PIFACE ),
	NAMEandVALUE( WPI_MODE_UNINITIALISED ),

// Pin modes
	NAMEandVALUE( INPUT ),
	NAMEandVALUE( OUTPUT ),
	NAMEandVALUE( PWM_OUTPUT ),
	NAMEandVALUE( GPIO_CLOCK ),
	NAMEandVALUE( SOFT_PWM_OUTPUT ),
	NAMEandVALUE( SOFT_TONE_OUTPUT ),
	NAMEandVALUE( PWM_TONE_OUTPUT ),

	NAMEandVALUE( LOW ),
	NAMEandVALUE( HIGH ),

// Pull up/down/none
	NAMEandVALUE( PUD_OFF ),
	NAMEandVALUE( PUD_DOWN ),
	NAMEandVALUE( PUD_UP ),

// PWM
	NAMEandVALUE( PWM_MODE_MS ),
	NAMEandVALUE( PWM_MODE_BAL ),

// Interrupt levels
	NAMEandVALUE( INT_EDGE_SETUP ),
	NAMEandVALUE( INT_EDGE_FALLING ),
	NAMEandVALUE( INT_EDGE_RISING ),
	NAMEandVALUE( INT_EDGE_BOTH ),

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.
	NAMEandVALUE( PI_MODEL_A ),
	NAMEandVALUE( PI_MODEL_B ),
	NAMEandVALUE( PI_MODEL_AP ),
	NAMEandVALUE( PI_MODEL_BP ),
	NAMEandVALUE( PI_MODEL_2 ),
	NAMEandVALUE( PI_ALPHA ),
	NAMEandVALUE( PI_MODEL_CM ),
	NAMEandVALUE( PI_MODEL_07 ),
	NAMEandVALUE( PI_MODEL_3B ),
	NAMEandVALUE( PI_MODEL_ZERO ),
	NAMEandVALUE( PI_MODEL_CM3 ),
	NAMEandVALUE( PI_MODEL_ZERO_W ),
	NAMEandVALUE( PI_MODEL_3BP ),
	NAMEandVALUE( PI_MODEL_3AP ),
	NAMEandVALUE( PI_MODEL_CM3P ),
	NAMEandVALUE( PI_MODEL_4B ),

	NAMEandVALUE( PI_VERSION_1 ),
	NAMEandVALUE( PI_VERSION_1_1 ),
	NAMEandVALUE( PI_VERSION_1_2 ),
	NAMEandVALUE( PI_VERSION_2 ),

	NAMEandVALUE( PI_MAKER_SONY ),
	NAMEandVALUE( PI_MAKER_EGOMAN ),
	NAMEandVALUE( PI_MAKER_EMBEST ),
	NAMEandVALUE( PI_MAKER_UNKNOWN ),

	NAMEandVALUE( LSBFIRST ),
	NAMEandVALUE( MSBFIRST ),

	{ NULL }
};

/* {{{ void wiringpi_test1()
 */
PHP_FUNCTION(wiringpi_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "wiringpi");
}
/* }}} */

/* {{{ string wiringpi_test2( [ string $var ] )
 */
PHP_FUNCTION(wiringpi_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

PHP_METHOD( wiringpi, setmode )
{
	zend_long pinmode;

	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pinmode )
	ZEND_PARSE_PARAMETERS_END();

	int result = -1;
	if ( pinmode == WPI_MODE_PINS )
	  result = wiringPiSetup();
	else if ( pinmode == WPI_MODE_GPIO )
	  result = wiringPiSetupGpio();
	else if ( pinmode == WPI_MODE_GPIO_SYS )
	  result = wiringPiSetupSys();
	else if ( pinmode == WPI_MODE_PHYS )
	  result = wiringPiSetupPhys();
	else if ( pinmode == WPI_MODE_PIFACE )
	  result = wiringPiSetupPiFace();

	RETURN_LONG( result );
}


PHP_METHOD( wiringpi, setup )
{
	zend_long pinno, pinmode, pud = PUD_OFF;

	ZEND_PARSE_PARAMETERS_START( 2, 3 )
		Z_PARAM_LONG( pinno )
		Z_PARAM_LONG( pinmode )
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG( pud )
	ZEND_PARSE_PARAMETERS_END();

	pinMode( pinno, pinmode );
	pullUpDnControl( pinno, pud );

	RETURN_NULL();
}


PHP_METHOD( wiringpi, input )
{
	zend_long pinno;

	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pinno )
	ZEND_PARSE_PARAMETERS_END();

	int result = digitalRead( pinno );

	RETURN_LONG( result );
}


PHP_METHOD( wiringpi, output )
{
	zend_long pinno, value;

	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pinno )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();

	digitalWrite( pinno, value );

	RETURN_NULL();
}


PHP_METHOD( wiringpi, wiringPiVersion )
{
	int major, minor;
	ZEND_PARSE_PARAMETERS_NONE();
	wiringPiVersion	( &major, &minor );
	array_init( return_value );
	add_assoc_long( return_value, "major", major );
	add_assoc_long( return_value, "minor", minor );
}


PHP_METHOD( wiringpi, wiringPiSetup )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( wiringPiSetup() );
}


PHP_METHOD( wiringpi, wiringPiSetupSys )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( wiringPiSetupSys() );
}


PHP_METHOD( wiringpi, wiringPiSetupGpio )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( wiringPiSetupGpio() );
}


PHP_METHOD( wiringpi, wiringPiSetupPhys )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( wiringPiSetupPhys() );
}


PHP_METHOD( wiringpi, pinModeAlt )
{
	zend_long pin, mode;

	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( mode )
	ZEND_PARSE_PARAMETERS_END();
	pinModeAlt( pin, mode );
}


PHP_METHOD( wiringpi, pinMode )
{
	zend_long pin, mode;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( mode )
	ZEND_PARSE_PARAMETERS_END();
	pinMode( pin, mode);
}


PHP_METHOD( wiringpi, pullUpDnControl )
{
	zend_long pin, pud;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( pud )
	ZEND_PARSE_PARAMETERS_END();
	pullUpDnControl( pin, pud);
}


PHP_METHOD( wiringpi, digitalRead )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( digitalRead( pin) );
}


PHP_METHOD( wiringpi, digitalWrite )
{
	zend_long pin, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	digitalWrite( pin, value);
}


PHP_METHOD( wiringpi, digitalRead8 )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( digitalRead8( pin ) );
}

PHP_METHOD( wiringpi, digitalWrite8 )
{
	zend_long pin, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	digitalWrite8( pin, value);
}


PHP_METHOD( wiringpi, pwmWrite )
{
	zend_long pin, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	pwmWrite( pin, value );
}


PHP_METHOD( wiringpi, analogRead )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG(  analogRead( pin ) );
}


PHP_METHOD( wiringpi, analogWrite )
{
	zend_long pin, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	analogWrite( pin, value);
}


PHP_METHOD( wiringpi, piGpioLayout )
{
	RETURN_LONG( piGpioLayout() );
}


PHP_METHOD( wiringpi, piBoardId )
{
	int model, rev, mem, maker, overVolted;
	piBoardId( &model, &rev, &mem, &maker, &overVolted );
	array_init( return_value );
	add_assoc_long( return_value, "model", model );
	add_assoc_long( return_value, "rev", rev );
	add_assoc_long( return_value, "mem", mem );
	add_assoc_long( return_value, "maker", maker );
	add_assoc_long( return_value, "overVolted", overVolted );
}


PHP_METHOD( wiringpi, wpiPinToGpio )
{
	zend_long wpiPin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( wpiPin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wpiPinToGpio( wpiPin ) );
}


PHP_METHOD( wiringpi, physPinToGpio )
{
	zend_long physPin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( physPin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( physPinToGpio( physPin ) );
}


PHP_METHOD( wiringpi, setPadDrive )
{
	zend_long group, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( group )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	setPadDrive( group, value );
}


PHP_METHOD( wiringpi, getAlt )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( getAlt( pin ) );
}


PHP_METHOD( wiringpi, pwmToneWrite )
{
	zend_long pin, freq;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( freq )
	ZEND_PARSE_PARAMETERS_END();
	pwmToneWrite( pin, freq );
}


PHP_METHOD( wiringpi, pwmSetMode )
{
	zend_long mode;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( mode )
	ZEND_PARSE_PARAMETERS_END();
	pwmSetMode( mode );
}


PHP_METHOD( wiringpi, pwmSetRange )
{
	zend_long range;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( range )
	ZEND_PARSE_PARAMETERS_END();
	pwmSetRange( range );
}


PHP_METHOD( wiringpi, pwmSetClock )
{
	zend_long divisor;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( divisor )
	ZEND_PARSE_PARAMETERS_END();
	pwmSetClock( divisor );
}


PHP_METHOD( wiringpi, gpioClockSet )
{
	zend_long pin, freq;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( freq )
	ZEND_PARSE_PARAMETERS_END();
	gpioClockSet( pin, freq);
}


PHP_METHOD( wiringpi, digitalReadByte )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( digitalReadByte() );
}


PHP_METHOD( wiringpi, digitalReadByte2 )
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG( digitalReadByte2() );
}


PHP_METHOD( wiringpi, digitalWriteByte )
{
	zend_long value;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	digitalWriteByte( value );
}


PHP_METHOD( wiringpi, digitalWriteByte2 )
{
	zend_long value;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	digitalWriteByte2( value );
}


PHP_METHOD( wiringpi, softPwmCreate )
{
	zend_long pin, value, range;

	ZEND_PARSE_PARAMETERS_START( 3, 3 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
		Z_PARAM_LONG( range )
	ZEND_PARSE_PARAMETERS_END();

	//ZVAL_LONG(return_value, softPwmCreate( pin, value, range) );
	RETURN_LONG( softPwmCreate( pin, value, range) );
}


PHP_METHOD( wiringpi, softPwmWrite )
{
	zend_long pin, value;

	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();
	softPwmWrite( pin, value);
	RETURN_NULL();
}


PHP_METHOD( wiringpi, softPwmStop )
{
	zend_long pin;

	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	softPwmStop( pin );
	RETURN_NULL();
}


PHP_METHOD( wiringpi, pcf8574Setup )
{
	zend_long pinBase, i2cAddress;

	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pinBase )
		Z_PARAM_LONG( i2cAddress )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( pcf8574Setup( pinBase, i2cAddress ) );
}


PHP_METHOD( wiringpi, pcf8591Setup )
{
	zend_long pinBase, i2cAddress;

	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pinBase )
		Z_PARAM_LONG( i2cAddress )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( pcf8591Setup( pinBase, i2cAddress ) );
}


PHP_METHOD( wiringpi, softServoWrite )
{
	zend_long pin, value;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( value )
	ZEND_PARSE_PARAMETERS_END();

	softServoWrite( pin, value);
	RETURN_NULL();
}


PHP_METHOD( wiringpi, softServoSetup )
{
	zend_long p0, p1, p2, p3, p4, p5, p6, p7;
	ZEND_PARSE_PARAMETERS_START( 7, 7 )
		Z_PARAM_LONG( p0 )
		Z_PARAM_LONG( p1 )
		Z_PARAM_LONG( p2 )
		Z_PARAM_LONG( p3 )
		Z_PARAM_LONG( p4 )
		Z_PARAM_LONG( p5 )
		Z_PARAM_LONG( p6 )
		Z_PARAM_LONG( p7 )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( softServoSetup( p0, p1, p2, p3, p4, p5, p6, p7 ) );
}


PHP_METHOD( wiringpi, softToneCreate )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( softToneCreate( pin ) );
}


PHP_METHOD( wiringpi, softToneStop )
{
	zend_long pin;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( pin )
	ZEND_PARSE_PARAMETERS_END();
	softToneStop( pin );
	RETURN_NULL();
}


PHP_METHOD( wiringpi, softToneWrite )
{
	zend_long pin, freq;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( pin )
		Z_PARAM_LONG( freq )
	ZEND_PARSE_PARAMETERS_END();
	softToneWrite( pin, freq );
	RETURN_NULL();
}


PHP_METHOD( wiringpi, wiringPiI2CRead )
{
	zend_long fd;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( fd )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CRead( fd ) );
}


PHP_METHOD( wiringpi, wiringPiI2CReadReg8 )
{
	zend_long fd, reg;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( fd )
		Z_PARAM_LONG( reg )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CReadReg8( fd, reg ) );
}


PHP_METHOD( wiringpi, wiringPiI2CReadReg16 )
{
	zend_long fd, reg;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( fd )
		Z_PARAM_LONG( reg )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CReadReg16( fd, reg ) );
}


PHP_METHOD( wiringpi, wiringPiI2CWrite )
{
	zend_long fd, data;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_LONG( fd )
		Z_PARAM_LONG( data )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CWrite( fd, data ) );
}


PHP_METHOD( wiringpi, wiringPiI2CWriteReg8 )
{
	zend_long fd, reg, data;
	ZEND_PARSE_PARAMETERS_START( 3, 3 )
		Z_PARAM_LONG( fd )
		Z_PARAM_LONG( reg )
		Z_PARAM_LONG( data )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CWriteReg8( fd, reg, data ) );
}


PHP_METHOD( wiringpi, wiringPiI2CWriteReg16 ) {
	zend_long fd, reg, data;
	ZEND_PARSE_PARAMETERS_START( 3, 3 )
		Z_PARAM_LONG( fd )
		Z_PARAM_LONG( reg )
		Z_PARAM_LONG( data )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CWriteReg16 ( fd, reg, data ) );
}


PHP_METHOD( wiringpi, wiringPiI2CSetupInterface )
{
	zend_string* device;
	zend_long devId;
	ZEND_PARSE_PARAMETERS_START( 2, 2 )
		Z_PARAM_STR( device )
		Z_PARAM_LONG( devId )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CSetupInterface( ZSTR_VAL(device ), devId ) );
}


PHP_METHOD( wiringpi, wiringPiI2CSetup ) {
	zend_long devId;
	ZEND_PARSE_PARAMETERS_START( 1, 1 )
		Z_PARAM_LONG( devId )
	ZEND_PARSE_PARAMETERS_END();
	RETURN_LONG( wiringPiI2CSetup( devId ) );
}


PHP_METHOD( wiringpi, getConstants )
{
	int n;
	char name[120];

	array_init( return_value );
	for ( n = 0; constants[n].name; n ++ ) {
		sprintf( name, WIRINGPI_CLASS_NAME "::%s", constants[n].name );
		add_assoc_long( return_value, name, constants[n].value );
	}
}


const zend_function_entry wiringpi_methods[] = {
	PHP_ME( wiringpi, setmode,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, setup,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, input,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, output,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiVersion,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiSetup,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiSetupSys,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiSetupGpio,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiSetupPhys,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pinModeAlt,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pinMode,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pullUpDnControl,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalRead,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalWrite,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalRead8,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalWrite8,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pwmWrite,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, analogRead,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, analogWrite,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, piGpioLayout,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, piBoardId,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wpiPinToGpio,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, physPinToGpio,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, setPadDrive,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, getAlt,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pwmToneWrite,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pwmSetMode,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pwmSetRange,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pwmSetClock,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, gpioClockSet,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalReadByte,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalReadByte2,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalWriteByte,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, digitalWriteByte2,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softPwmCreate,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softPwmWrite,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softPwmStop,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pcf8574Setup,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, pcf8591Setup,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softServoWrite,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softServoSetup,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softToneCreate,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softToneStop,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, softToneWrite,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, getConstants,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )

	PHP_ME( wiringpi, wiringPiI2CRead,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CReadReg8,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CReadReg16,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CWrite,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CWriteReg8,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CWriteReg16,		NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CSetupInterface,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_ME( wiringpi, wiringPiI2CSetup,				NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_FE_END	/* Must be the last line in myext_functions[] */
};

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(wiringpi)
{
#if defined(ZTS) && defined(COMPILE_DL_WIRINGPI)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ REGISTER_WIRINGPI_CLASS_CONST_LONG */
#define REGISTER_WIRINGPI_CLASS_CONST_LONG(const_name, value) \
		zend_declare_class_constant_long( wiringpi_class_entry, const_name, strlen(const_name), (zend_long)value );
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
static PHP_MINIT_FUNCTION(wiringpi)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY( ce, WIRINGPI_CLASS_NAME, wiringpi_methods );
	wiringpi_class_entry = zend_register_internal_class(&ce);
	int n;

//	zend_declare_property_string( wiringpi_class_entry, "hello", sizeof("hello") - 1, "hello, WiringPi!", ZEND_ACC_PUBLIC);

	for ( n = 0; constants[n].name; n ++ ) {
		REGISTER_WIRINGPI_CLASS_CONST_LONG( constants[n].name, constants[n].value );
	}
	return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wiringpi)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Variable", "Value");
	php_info_print_table_row(2, WIRINGPI_CLASS_NAME " support", "enabled");
	php_info_print_table_row(2, WIRINGPI_CLASS_NAME " Version", PHP_WIRINGPI_VERSION);
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_wiringpi_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wiringpi_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ wiringpi_functions[]
 */
static const zend_function_entry wiringpi_functions[] = {
	PHP_FE(wiringpi_test1,		arginfo_wiringpi_test1)
	PHP_FE(wiringpi_test2,		arginfo_wiringpi_test2)
	PHP_FE_END
};
/* }}} */

/* {{{ wiringpi_module_entry
 */
zend_module_entry wiringpi_module_entry = {
	STANDARD_MODULE_HEADER,
	WIRINGPI_CLASS_NAME,			/* Extension name */
	wiringpi_functions,				/* zend_function_entry */
	PHP_MINIT(wiringpi),			/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(wiringpi),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(wiringpi),			/* PHP_MINFO - Module info */
	PHP_WIRINGPI_VERSION,			/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WIRINGPI
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(wiringpi)
#endif

