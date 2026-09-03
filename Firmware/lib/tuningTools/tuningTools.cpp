#include <tuningTools.h>
BluetoothSerial SerialBT;



int includedIn(String string1, String string2) {
    if (string2.indexOf(string1) == -1) //If string2 is not included in string1
        return 0;
    return 1;
}



float getValue(String text) {
    //Obtains a float inside the string, if there is none returns 0.
    int sgn = 1;
    if (text.indexOf('-')!=-1) //If text contains '-'
      sgn = -1;
    while (text != "" && !isDigit(text[0]))
            text.remove(0, 1);
    //This function needs to start with a digit, so we removed all the non-digits at the start of the string        
    return (float)(sgn*(text.toFloat()));

}

