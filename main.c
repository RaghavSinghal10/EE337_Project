#include <at89c5131.h>
#include "lcd.h"		   //Header file with LCD interfacing functions
#include "serial.c"	   //C file with UART interfacing functions
#include <stdio.h>
#include <string.h>

//initialize global variables
int runs = 0;
int wickets = 0;
int balls = 0;
int overs = 0;
char over_state[16];   //stores what happened in the over in a string
char score[16];        //stores score in a string

//gets the cureent score in the form of a string
void get_score(void)
{
	//make a copy of the integer variables
	int runs_copy = runs;
	int wickets_copy = wickets;
	int overs_copy = overs;
	int balls_copy = balls;
	
	//ch_score stores the character to be appended
	//temp, digit, factor are used to extract digits from a number
	char ch_score;                           
	int temp, digit, factor=1;
	
	//clear score string
	score[0] = 0;
	
	//extract the digits from runs and append to string
	//*****************************
	temp = runs_copy;
		if (temp == 0)
		{
			ch_score = '0';
			strncat(score, &ch_score, 1);
		}
    while(temp)
    {
        temp = temp/10;
        factor = factor*10;
    }   
    while(factor>1)
    {
        factor = factor/10;
        digit = runs_copy/factor;
        ch_score = digit + '0';
        strncat(score, &ch_score, 1);
        runs_copy = runs_copy % factor;
    }
		//***************************
    
		//concatenate '/' to the string
    ch_score = '/';
    strncat(score, &ch_score, 1);
    
		//extract the digits from wickets and append to string
		//*****************************
    temp = wickets_copy;
		if (temp == 0)
		{
			ch_score = '0';
			strncat(score, &ch_score, 1);
		}
    factor = 1;
    while(temp)
    {
        temp = temp/10;
        factor = factor*10;
    }   
    while(factor>1)
    {
        factor = factor/10;
        digit = wickets_copy/factor;
        ch_score = digit + '0';
        strncat(score, &ch_score, 1);
        wickets_copy = wickets_copy % factor;
    }
		//*****************************
    
		//concatenate ' ' to the string
    ch_score = ' ';
    strncat(score, &ch_score, 1);
    
		//concatenate '(' to the string
    ch_score = '(';
    strncat(score, &ch_score, 1);
    
		//extract the digits from overs and append to string
		//*****************************
    temp = overs_copy;
		if (temp == 0)
		{
			ch_score = '0';
			strncat(score, &ch_score, 1);
		}
    factor = 1;
    while(temp)
    {
        temp = temp/10;
        factor = factor*10;
    }
    
    while(factor>1)
    {
        factor = factor/10;
        digit = overs_copy/factor;
        ch_score = digit + '0';
        strncat(score, &ch_score, 1);
        overs_copy = overs_copy % factor;
    }
		//*****************************
    
		//concatenate '.' to the string
    ch_score = '.';
    strncat(score, &ch_score, 1);
    
		//concatenate balls to the string
    ch_score = balls_copy + '0';
    strncat(score, &ch_score, 1);
    
		//concatenate ')' to the string
    ch_score = ')';
    strncat(score, &ch_score, 1);
    
}

//displays the score on the first line
void display_score(void)
{
	lcd_cmd(0x01);               //clear lcd
	lcd_cmd(0x80);               //move to starting of the first line
	lcd_write_string(score);
}

//displays what happened in the over on the second line
void display_over_state(void)
{
	lcd_cmd(0xC0);               //move to starting of the second line
	lcd_write_string(over_state);
}

//checks if over is complete
//updates variables accordingly
//calls the functioncs to display the score and events of the over
void check_over(void)
{
	//if over is completed
	if (balls == 6)
	{
		balls = 0;                 //set balls to 0
		overs = overs + 1;         //increment overs
		over_state[0] = 0;         //empty the over_state string   
	}
	//get the score, display score and over events
	get_score();               
	display_score();
	display_over_state();
}

//checks if innnings is over
//updates variables and calls functions to display on lcd accordinlgy
void check_innings(void)
{
	//if innings is over
	if ((overs == 20) || (wickets == 10))
	{	
		lcd_cmd(0xC0);              //move to starting of second line
		
		//display "Innings Ends" for 5 seconds
		lcd_write_string("Innings Ends");
		msdelay(5000);
		
		//reset global variables
		runs = 0;
		balls = 0;
		wickets = 0;
		overs = 0;
		over_state[0] = 0;
		score[0] = 0;
		
		//start of new innings
		get_score();
		display_score();
		display_over_state();
	}
}
		

//Main function
void main(void)
{
	
	unsigned char ch = 0;
	
	//Call initialization functions
	lcd_init();
	uart_init();
	
	//display "0/0 (0.0)" at start on the first line
	lcd_cmd(0x80);
	lcd_write_string("0/0 (0.0)");
	
	while(1)
	{
			//Receive a character
			ch = receive_char();
		
			switch(ch)
			{
				//1 run
				case '1':runs = runs + 1;
								 balls = balls + 1;
					       strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
								 break;
				
				//2 runs
				case '2':runs = runs + 2;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
								 break;
				
				//3 runs
				case '3':runs = runs + 3;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
				
				//4 runa
				case '4':runs = runs + 4;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
				//5 runs				 
				case '5':runs = runs + 5;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
								 
				//6 runs				 
				case '6':runs = runs + 6;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
								 
				//dot ball				 
				case '.':balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
								 
				//wicket				 
				case 'w':wickets = wickets + 1;
								 balls = balls + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
				
				//wide
				case 'e':runs = runs + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
				
				//no ball
				case 'n':runs = runs + 1;
								 strncat(over_state, &ch, 1);
								 check_over();
								 check_innings();
							   break;
				
				//if wrong key is pressed
				default:transmit_string("Incorrect key. Press correct key.\r\n");
								break;
				
			}
			msdelay(100);
	}
}
