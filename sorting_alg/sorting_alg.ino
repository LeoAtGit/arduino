//TODO
// -implement different sorting algos and make them switchable with a button

// If not commented out, you will get output to Serial(9600)
//#define DEBUG

// Maximum brightness of the LEDs
#define MAX_BRIGHTNESS 255 
// Number of LEDs in the cluster
#define NUM_OF_LED 6
// Defines the delay between updating the list and displaying the results with
// the LEDs. 1000 = 1 sec
#define DELAY_SORT_BY 1000
// Define the delay between the scrambling of the LEDs and the actual sorting
#define DELAY_BETWEEN_NEW_SORT 5000
// The number of an unused analog pin used for seeding the rng
#define NOT_USED_ANALOG_PIN 4

// List of leds and the port they are connected to
int LED[NUM_OF_LED] = {11, 10, 9, 6, 5, 3};
// List that will get sorted
int RAND_LIST[NUM_OF_LED];

void setup()
{
	int i;

	randomSeed(analogRead(NOT_USED_ANALOG_PIN));

#ifdef DEBUG
	Serial.begin(9600);
	while (!Serial) {;}
#endif

	for (i = 0; i < NUM_OF_LED; i++) {
		pinMode(LED[i], OUTPUT);
	}
}

void loop()
{
	// Make random LEDs light up
	fill_rand_list();
	apply_list_to_leds();

	delay(DELAY_BETWEEN_NEW_SORT); 

	// Sort the LEDs by intensity
	bubble_sort();
}

/**
 * Simple implementation of the bubble sort algorithm
 * Sorts the RAND_LIST list
 */
void bubble_sort()
{
	int i, j, tmp, fin;

	fin = 1;
	i = 1;
	for (;;) {
		// check if the list is finished sorting
		for (j = 1; j < NUM_OF_LED; j++) {
			if (!(RAND_LIST[j] >= RAND_LIST[j - 1])) {
				fin = 0;
			}
		}

		tmp = RAND_LIST[i];
		if (tmp < RAND_LIST[i - 1]) {
			RAND_LIST[i] = RAND_LIST[i - 1];
			RAND_LIST[i - 1] = tmp;
		}

		if (fin) {
			break;
		} else {
			fin = 1;
			// loop the i back around
			i = ((i + 1) == NUM_OF_LED) ? 1 : i + 1;
		}

		apply_list_to_leds();
		delay(DELAY_SORT_BY);
	}
}

/**
 * Function that initialises the list that will later be sorted
 * Seed with randomSeed(analogRead(NOT_USED_ANALOG_PIN)) or similar
 * Fills the list with values from 0 to MAX_BRIGHTNESS (best case, actual
 * results may differ because I won't do floating point arithmetic here)
 */
void fill_rand_list()
{
	int i, j, tmp;

	for (i = 0; i < NUM_OF_LED; i++) {
		RAND_LIST[i] = (MAX_BRIGHTNESS / (NUM_OF_LED - 1)) * random(NUM_OF_LED);
		tmp = RAND_LIST[i];
		j = i;
		while (--j >= 0) {
			if (RAND_LIST[j] == tmp) {
				--i;
				break;
			}
		}
	}
}

/**
 * Function that applies the voltage in RAND_LIST to the actual connected LEDs
 */
void apply_list_to_leds()
{
	int i;

	for (i = 0; i < NUM_OF_LED; i++) {
		analogWrite(LED[i], RAND_LIST[i]);
#ifdef DEBUG
		Serial.println(RAND_LIST[i]);
#endif
	}
}

