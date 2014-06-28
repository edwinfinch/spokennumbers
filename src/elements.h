Window *window;
TextLayer *word_1, *word_2, *word_3, *word_4;

InverterLayer *theme;

GFont *gotham_bold_c, *gotham_bold_c1;

int minute, hour;
bool isFreshHour, hourIsOver10, minuteIsTeens;

char word1_buffer[] = "Rick and Morty";
char word2_buffer[] = "rickandmorty.com";
char word3_buffer[] = "R && M forever";
char word4_buffer[] = "Test post...";

char *teens[] = {
	"ELEVEN",
	"TWELVE",
	"THIRTEEN",
	"FOURTEEN",
	"FIFTEEN",
	"SIXTEEN",
	"SEVENTEEN",
	"EIGHTEEN",
	"NINETEEN",
};

char *tens[] = {
	"ZERO",
	"TEN",
	"TWENTY",
	"THIRTY",
	"FOURTY",
	"FIFTY",
};

char *extra_hour_words[] = {
	"TWENTY ONE",
	"TWENTY TWO",
	"TWENTY THREE",
	"TWENTY FOUR",
};

char *ones[] = {
	"",
	"ONE",
	"TWO",
	"THREE",
	"FOUR",
	"FIVE",
	"SIX",
	"SEVEN",
	"EIGHT",
	"NINE",
	"TEN"
};