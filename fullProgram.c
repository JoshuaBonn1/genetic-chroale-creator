#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"stats.h"
#include<math.h>

double findFitness(int **chorale, int *chord_prog, int last);
int compare(const void * a, const void * b);
void select_sort(int *** chorales, double * fitness, int size);
void makeFile(int **chorale, int *chordProg);
void crossBlock(int **mom, int **dad, int **baby1, int **baby2);
void crossLine(int **mom, int **dad, int **baby1, int **baby2);
void crossIndiv(int **mom, int **dad, int **baby1, int **baby2);
void generate(int ***chorales, double *fitness);
int randNote(int voice);
void add_chord(int **chorale, int *chords, int position);
int chord_type(int position);
void create_random_chorale(int **chorale, int *chords);

#define NUM_OF_CHORDS 113
#define CW CHORD_WHEEL
int CHORD_WHEEL[NUM_OF_CHORDS][4] = {
    {1, 5, 8, 10},
    {1, 3, 8, 12},
    {3, 5, 8, 12},
    {1, 5, 10, 12},
    {1, 8, 10, 12},
    {5, 8, 10, 12},
    {1, 5, 10, 15},
    {5, 8, 10, 15},
    {3, 8, 12, 15},
    {1, 10, 12, 15},
    {5, 10, 12, 15},
    {8, 10, 12, 15},
    {1, 8, 12, 17},
    {5, 8, 12, 17},
    {1, 12, 15, 17},
    {5, 12, 15, 17},
    {8, 12, 15, 17},
    {1, 10, 15, 19},
    {5, 10, 15, 19},
    {8, 10, 15, 19},
    {3, 12, 15, 19},
    {10, 12, 15, 19},
    {1, 12, 17, 19},
    {8, 12, 17, 19},
    {4, 6, 9, 11},
    {4, 9, 11, 13},
    {4, 6, 11, 16},
    {4, 11, 13, 16},
    {6, 9, 11, 18},
    {4, 9, 13, 18},
    {2, 11, 13, 18},
    {9, 11, 13, 18},
    {6, 11, 16, 18},
    {4, 11, 16, 20},
    {0, 5, 7, 10},
    {3, 5, 7, 10},
    {0, 3, 7, 12},
    {0, 7, 10, 12},
    {3, 7, 10, 12},
    {0, 5, 10, 14},
    {3, 5, 10, 14},
    {5, 7, 10, 14},
    {3, 7, 12, 14},
    {0, 10, 12, 14},
    {3, 10, 12, 14},
    {7, 10, 12, 14},
    {0, 7, 12, 17},
    {3, 7, 12, 17},
    {0, 10, 12, 17},
    {7, 10, 12, 17},
    {5, 10, 14, 17},
    {0, 12, 14, 17},
    {3, 12, 14, 17},
    {7, 12, 14, 17},
    {10, 12, 14, 17},
    {0, 10, 14, 19},
    {3, 10, 14, 19},
    {7, 10, 14, 19},
    {0, 12, 17, 21},
    {3, 12, 17, 21},
    {7, 12, 17, 21},
    {10, 12, 17, 21},
    {1, 6, 8, 11},
    {4, 6, 8, 11},
    {1, 4, 8, 13},
    {1, 8, 11, 13},
    {4, 8, 11, 13},
    {1, 6, 11, 15},
    {4, 6, 11, 15},
    {6, 8, 11, 15},
    {4, 8, 13, 15},
    {1, 11, 13, 15},
    {4, 11, 13, 15},
    {8, 11, 13, 15},
    {1, 8, 13, 18},
    {4, 8, 13, 18},
    {1, 11, 13, 18},
    {8, 11, 13, 18},
    {6, 11, 15, 18},
    {1, 11, 15, 20},
    {4, 11, 15, 20},
    {8, 11, 15, 20},
    {2, 5, 7, 12},
    {0, 5, 9, 12},
    {5, 7, 9, 12},
    {5, 9, 12, 14},
    {5, 12, 14, 16},
    {2, 12, 14, 19},
    {9, 12, 14, 19},
    {0, 12, 16, 19},
    {7, 12, 16, 19},
    {5, 14, 16, 19},
    {3, 6, 8, 10},
    {3, 8, 10, 13},
    {3, 6, 10, 15},
    {3, 10, 13, 15},
    {3, 8, 13, 17},
    {1, 10, 13, 17},
    {8, 10, 13, 17},
    {6, 10, 15, 17},
    {3, 13, 15, 17},
    {10, 13, 15, 17},
    {3, 10, 15, 20},
    {0, 7, 9, 11},
    {4, 7, 9, 14},
    {2, 7, 11, 14},
    {0, 9, 11, 14},
    {7, 9, 11, 14},
    {0, 11, 14, 16},
    {7, 11, 14, 16},
    {0, 14, 16, 18},
    {7, 14, 16, 18},
    {4, 14, 16, 21},
};

double findFitness(int **chorale, int *chords, int last);

#define FILE_NAME "chorale.ly"

void makeFile(int **chorale, int *chordProg){
    FILE *fp;

    fp = fopen(FILE_NAME, "w");

    //Header
    fputs("\\paper {\n  top-system-spacing #'basic-distance = #10\n  score-system-spacing #'basic-distance = #20\n  system-system-spacing #'basic-distance = #20\n  last-bottom-spacing #'basic-distance = #10\n}\n\nglobal = {\n  \\key c \\major\n  \\time 4/4\n}\n\n", fp);

    int part,i;
    //Each index of string associates with the character
    char *numToNote = "gabcdefgabcdefgabcdefg";
    for(part = 0; part < 4; part++){
        int currNote;

        //store starting note into currNote to use relativity
        switch(part){
             case 0: currNote = 17;
                    fputs("sopMusic = \\relative c'' {\n\t", fp);
                    break;
            case 1: currNote = 10;
                    fputs("altoMusic = \\relative c' {\n\t", fp);
                    break;
            case 2: currNote = 10;
                    fputs("tenorMusic = \\relative c' {\n\t", fp);
                    break;
            case 3: currNote = 3;
                    fputs("bassMusic = \\relative c {\n\t", fp);
                    break;
        }

        //Check to make sure next note goes up or down an octave as needed
        for(i = 0; i < LENGTH; i++){
            fputc(numToNote[chorale[part][i]], fp);
            if(chorale[part][i] > currNote + 3)//Goes up a fifth or more
                fputc('\'', fp);//Write to file
            if(chorale[part][i] < currNote - 3)//Goes down a fifth or more
                fputc(',', fp);//Write to file
            currNote = chorale[part][i];
            fputc(' ', fp);
        }
        fputs("\n}\n", fp);
    }

    fputs("\\score {\n  <<\n    \\new PianoStaff <<\n      \\new Staff <<\n        \\set Staff.printPartCombineTexts = ##f\n        \\partcombine\n        << \\global \\sopMusic >>\n        << \\global \\altoMusic >>\n      >>\n      \\new Staff <<\n        \\clef bass\n        \\set Staff.printPartCombineTexts = ##f\n        \\partcombine\n        << \\global \\tenorMusic >>\n        << \\global \\bassMusic >>\n      >>\n    ", fp);
    fputs("  \\new FiguredBass \\figuremode {\n         \\set figuredBassAlterationDirection = #RIGHT\n         \\set figuredBassPlusDirection = #RIGHT\n       \\override BassFigureAlignment.stacking-dir = #DOWN\n", fp);
    fputs("             ", fp);

    for(i = 0; i < LENGTH; i++){
        fputc('<', fp);
        if(chordProg[i] != -1)
            fprintf(fp, "%d", chordProg[i]);
        else
            fputc('_', fp);
        fputs(">4 ", fp);
    }

    //Footer
    fputs("\n      }\n    >>\n  >>\n  \\layout{}\n\\midi{}\n}", fp);
}




struct pos_fit_s{
    int pos;
    double fit;
};

void crossBlock(int ** mom, int ** dad, int ** baby1, int ** baby2){

    //Find a crossover point for beats
    int cross = rand()%LENGTH;
    int beat, voice;

    //Place mom and dad before crossover point into babies
    for(beat = 0; beat < cross; beat++){
        for(voice = 0; voice < 4; voice++){
            baby1[voice][beat] = mom[voice][beat];
            baby2[voice][beat] = dad[voice][beat];
        }
    }

    //Place mom and dad after crossover point into opposite babies
    for(beat = cross; beat < LENGTH; beat++){
        for(voice = 0; voice < 4; voice++){
            baby1[voice][beat] = dad[voice][beat];
            baby2[voice][beat] = mom[voice][beat];
        }
    }
}

void generate(int ***chorales, double *fitness){
    int new_chorale[POPULATION_SIZE][4][LENGTH];

    //Take top 10% of population
    int midpoint = POPULATION_SIZE * .1;
    struct pos_fit_s *pos_fit = (struct pos_fit_s *)malloc(3 * sizeof(struct pos_fit_s));

    int i;

    for(i = midpoint; i < POPULATION_SIZE; i+=2){
        //Select mom and dad chorale
        int mom_i = -1, dad_i = -1;
        int j;

        //if(rand()%32 == 1) mom_i = rand()%midpoint;
        //if(rand()%32 == 1) dad_i = rand()%midpoint;

        while(mom_i == -1 || dad_i == -1){
            for(j = 0; j < 3; j++){
                int r1 = rand()%POPULATION_SIZE;
                pos_fit[j].pos = r1;
                pos_fit[j].fit = fitness[r1];
            }
            double max_d = -100.0;
            double max_i = -1;
            for(j = 0; j < 3; j++){
                if(pos_fit[j].fit > max_d){
                    max_d = pos_fit[j].fit;
                    max_i = pos_fit[j].pos;
                }
            }

            if(mom_i == -1)
                mom_i = max_i;
            else
                dad_i = max_i;
        }

        //Allocating memory for babies
        int **baby1 = (int **)malloc(4 * sizeof(int *));
        int **baby2 = (int **)malloc(4 * sizeof(int *));
        int k;
        for(k = 0; k < 4; k++){
            baby1[k] = (int *)malloc(LENGTH * sizeof(int));
            baby2[k] = (int *)malloc(LENGTH * sizeof(int));
        }

        //Choose a crossover method
        crossBlock(chorales[mom_i], chorales[dad_i], baby1, baby2);


        //Mutations
        int beat, voice;

        for(beat = 0; beat < LENGTH; beat++){
                if(rand()%25 == 1) add_chord(baby1, CW[rand() % NUM_OF_CHORDS], beat);
                if(rand()%25 == 1) add_chord(baby2, CW[rand() % NUM_OF_CHORDS], beat);
        }

        //Place baby into chorales and free baby2 if no space left
        for(voice = 0; voice < 4; voice++){
            for(beat = 0; beat < LENGTH; beat++){
                new_chorale[i][voice][beat] = baby1[voice][beat];
	    }
        }

        if(i+1 != POPULATION_SIZE){
            //new_chorale[i+1] = baby2;
            for(voice = 0; voice < 4; voice++){
                for(beat = 0; beat < LENGTH; beat++)
                    new_chorale[i+1][voice][beat] = baby2[voice][beat];
            }
        }

        for(k = 0; k < 4; k++){
            free(baby1[k]);
            free(baby2[k]);
        }
        free(baby1);
        free(baby2);
    }

    for(i = midpoint; i < POPULATION_SIZE; i++){
        int v, b;
        for(v = 0; v < 4; v++){
            for(b = 0; b < LENGTH; b++)
                chorales[i][v][b] = new_chorale[i][v][b];
        }

    }

    free(pos_fit);
}

#define ERR_FILE "errors.txt"

#define REWARD_STARTING_NOTE 0//1
#define PUNISH_STARTING_NOTE -1//-.1

#define REWARD_ENDING_NOTE 0//1
#define PUNISH_ENDING_NOTE -1//-.1

#define REWARD_STARTING_CHORD 0//1
#define PUNISH_STARTING_CHORD -1//-.1

#define REWARD_ENDING_CHORD 0//1
#define PUNISH_ENDING_CHORD -1//-.1

#define REWARD_CHORD 0//1
#define PUNISH_NOT_CHORD -1//-.1

#define REWARD_CHORD_PROGRESSION 0//1
#define PUNISH_CHORD_PROGRESSION -1//-.1

#define REWARD_DOUBLING 0//1
#define PUNISH_DOUBLING -1//-.1

#define PUNISH_TENDENCY_TONES -1//-.2

#define PUNISH_WITHIN_OCTAVE -1//-.1

#define PUNISH_OVERLAP -1//-.2

#define PUNISH_DIRECT_5_8 -1//-.1

#define PUNISH_UNEQUAL_5 -1//-.1

#define PUNISH_PARALLEL_5_8 -1//-.1

#define PUNISH_CONTRARY_5_8 -1//-.1

//ADDED RULES

#define PUNISH_BIG_LEAP -1//-.1

#define PUNISH_REPEAT_CHORD 0//-1//-.2

#define PUNISH_TOO_MANY_LEAPS -1//-.1

#define PUNISH_TOO_MANY_ONES 0//-1//-.1

#define PUNISH_REPEAT_NOTES -1//-.1

#define REWARD_ROOT_POSITION 0//1
#define PUNISH_ROOT_POSITION 0//-1//-.1

int compare(const void * a, const void * b){
    return (*(int*)a - *(int*)b);
}

void voiceToName(int voice, FILE *fp){
    switch(voice){
        case 0: fputs("Soprano", fp); break;
        case 1: fputs("Alto", fp); break;
        case 2: fputs("Tenor", fp); break;
        case 3: fputs("Bass", fp); break;
    }
}

double findFitness(int **chorale, int *chord_prog, int last){

    FILE *fp;

    if(last) fp = fopen(ERR_FILE, "w");

    int leaps = 0;
    double fitness = 0;

    //Soprano starts on C or G
    if(chorale[0][0] % 7 == 3 || chorale[0][0] % 7 == 0)
        fitness += REWARD_STARTING_NOTE;
    else{
        fitness += PUNISH_STARTING_NOTE;
        if(last) fputs("Incorrect starting note.\n", fp);
    }

    //Soprano ends on C or G
    if(chorale[0][31] % 7 == 3 || chorale[0][31] % 7 == 0)
        fitness += REWARD_ENDING_NOTE;
    else{
        fitness += PUNISH_ENDING_NOTE;
        if(last) fputs("Incorrect ending note.\n", fp);
    }

    //Is A Chord
    int chordProg[LENGTH];
    int i, j, k;
    for(i = 0; i < LENGTH; i++){
        int numNotes = 4;

        //Load chord from vertical component of chorale
        int chord[4];
        for(j = 0; j < 4; j++) chord[j] = chorale[j][i] % 7;

        //Sort from lowest to highest
        qsort(chord, 4, sizeof(int), compare);

        //Remove Duplicates and keep track of # of notes
        for(j = 1; j < 4; j++){
            if(chord[j-1]==chord[j] && chord[j] != -1){
                for(k = j-1; k<3; k++)
                    chord[k] = chord[k+1];
                chord[k] = -1;
                j--;
                numNotes--;
            }
        }

        //Ignoring missing notes(e.g.the fifth from a I chord) and 7th chords right now
        if(numNotes != 3){
            if(last){
                fputs("Beat ", fp);
                fprintf(fp, "%d", i);
                fputs(": Not a chord. (May be 7th chord).\n", fp);
            }
            fitness += PUNISH_NOT_CHORD;
            chordProg[i]=-1;
        }
        else{
            //Keep track of intervals for easy typing
            int diff1 = (chord[1] > chord[0])? chord[1]-chord[0]: (chord[1]+7)-chord[0];
            int diff2 = (chord[2] > chord[1])? chord[2]-chord[1]: (chord[2]+7)-chord[1];

            //Check chord intervals to be correct(i.e. 2-2, 2-3, 3-2) and store bottom note
            switch(diff1){
                case 2: if(diff2==2||diff2==3)
                            chordProg[i]=(((diff2==2)?chord[0]:chord[2])+4)%7+1;
                        else{
                            if(last){
                                fputs("Beat ", fp);
                                fprintf(fp, "%d", i+1);
                                fputs(": Not a chord.\n", fp);
                            }

                            fitness += PUNISH_NOT_CHORD;
                            chordProg[i]=-1;
                        }
                        break;
                case 3: if(diff2==2){
                            chordProg[i]=(chord[1]+4)%7+1;
                        }
                        else{
                            if(last){
                                fputs("Beat ", fp);
                                fprintf(fp, "%d", i+1);
                                fputs(": Not a chord.\n", fp);
                            }
                            fitness += PUNISH_NOT_CHORD;
                            chordProg[i]=-1;
                        }
                        break;
                default: fitness += PUNISH_NOT_CHORD;
                         if(last){
                             fputs("Beat ", fp);
                             fprintf(fp, "%d", i+1);
                             fputs(": Not a chord.\n", fp);
                         }
                         chordProg[i]=-1;
            }//END switch
        }
    }

    //Set passed in chord progression for file printing
    for(i = 0;i < LENGTH; i++){
        if(chordProg[i] != -1) fitness+=REWARD_CHORD;
        chord_prog[i] = chordProg[i];
    }

    if(chordProg[0] == 1) fitness += REWARD_STARTING_CHORD;
    else{
        fitness += PUNISH_STARTING_CHORD;
        if(last) fputs("Bad starting chord.\n", fp);
    }

    if(chordProg[LENGTH-1] == 1) fitness += REWARD_ENDING_CHORD;
    else{
        fitness += PUNISH_ENDING_CHORD;
        if(last) fputs("Bad ending chord.\n", fp);
    }

    int repeat = 1;
    //Chord Progression and Repeats
    for(i = 0; i < LENGTH - 1; i++){
        int next = chordProg[i+1];
        int flag = 0;

        //Check if next chord is in chord progression
        if(next != -1){
            switch(chordProg[i]){
                case 2: if(next != 7 && next != 5) flag++; break;
                case 3: if(next != 4 && next != 6) flag++; break;
                case 4: if(next != 1 && next != 2 && next != 5 && next != 7) flag++; break;
                case 5: if(next != 1 && next != 6) flag++; break;
                case 6: if(next != 4 && next != 2) flag++; break;
                case 7: if(next != 1) flag++; break;
            }
        }

        if(flag == 0 && next != -1 && chordProg[i] != -1)
            fitness += REWARD_CHORD_PROGRESSION;

        if(flag && next != chordProg[i]){
            fitness += PUNISH_CHORD_PROGRESSION;
            if(last){
                fputs("Beat ", fp);
                fprintf(fp, "%d", i+1);
                fputc('-', fp);
                fprintf(fp, "%d", i+2);
                fputs(": Bad chord progression: ", fp);
                fprintf(fp, "%d", chordProg[i]);
                fputc('-', fp);
                fprintf(fp, "%d", chordProg[i+1]);
                fputc('\n', fp);
            }
        }

        if(next == chordProg[i]) repeat++;
        else repeat=1;
        if(repeat>2){
            fitness+=PUNISH_REPEAT_CHORD;
            if(last){
                fputs("Beat ", fp);
                fprintf(fp, "%d", i+2);
                fputs(": Repeated chord: ", fp);
                fprintf(fp, "%d", chordProg[i+1]);
                fputc('\n', fp);
            }
        }

        if(i > 1 && chordProg[i] == 1 && chordProg[i-2] == 1){
             fitness+=PUNISH_TOO_MANY_ONES;
        }

        if(chordProg[i] != (chorale[3][i] + 4) % 7 + 1){
            fitness+=PUNISH_ROOT_POSITION;
            if(last){
                fputs("Beat ", fp);
                fprintf(fp, "%d", i+1);
                fputs(": Not in root position\n", fp);
            }
        }
        else
            fitness+=REWARD_ROOT_POSITION;
    }

    //Doubling
    for(i = 0; i < LENGTH; i++){
        int flag = 0;
        if(chordProg[i] != -1){
            //Load chord from vertical component of chorale
            int chord[4];
            for(j = 0; j < 4; j++) chord[j] = (chorale[j][i]+4) % 7 + 1;

            qsort(chord, 4, sizeof(int), compare);

            //Find doubled note
            int duplicate = -1;
            for(j = 0; j < 3; j++){
                if(chord[j] == chord[j+1]) duplicate = chord[j];
            }

            switch(chordProg[i]){
                case 3: if(duplicate != 5 && duplicate != 3){
                            fitness += PUNISH_DOUBLING;
                            flag = 1;
                        }
                        break;
                case 4: if(duplicate != 1){
                            fitness += PUNISH_DOUBLING;
                            flag = 1;
                        }
                        break;
                case 7: if(duplicate != 2){
                            fitness += PUNISH_DOUBLING;
                            flag = 1;
                        }
                        break;
                default: if(duplicate != chordProg[i] && duplicate != (chordProg[i] + 4) % 7){
                             fitness += PUNISH_DOUBLING;
                             flag = 1;
                         }
            }//END switch

            if(flag == 0)
                fitness+= REWARD_DOUBLING;

            if(flag && last){
                fputs("Beat ", fp);
                fprintf(fp, "%d", i+1);
                fputs(": Bad duplication of ", fp);
                fprintf(fp, "%d", duplicate);
                fputs(" in chord ", fp);
                fprintf(fp, "%d", chordProg[i]);
                fputc('\n', fp);
            }
        }
    }

    repeat = 0;

    //Tendency Tones and Big Leaps
    for(i = 0; i < LENGTH - 1; i++){
        for(j = 0; j < 4; j++){
            int flag = 0;

            if(j == 0){
                if(chorale[j][i] == chorale[j][i+1])
                    repeat++;
                else
                    repeat=0;
                if(repeat>2){
                    fitness+=PUNISH_REPEAT_NOTES;
                }
            }

            int intervals[3];
            if(i > 0)
                intervals[0] = chorale[j][i-1] - chorale[j][i];
            else
                intervals[0] = 50;
            intervals[1] = chorale[j][i] - chorale[j][i+1];
            if(i < 30)
                intervals[2] = chorale[j][i+1] - chorale[j][i+2];
            else
                intervals[2] = 50;

            //Check approach and departure from leap in opposite direction
            if(intervals[1] > 3){
                if(intervals[0] == 50 || intervals[0] >= 0 || intervals[2] == 50 || intervals[2] >= 0)
                    flag = 2;
            }
            else if(intervals[1] < -3){
                if(intervals[0] == 50 || intervals[0] <= 0 || intervals[2] == 50 || intervals[2] <= 0)
                    flag = 2;
            }


            int prev;
            if(i > 0)
                prev = (chorale[j][i-1] + 4 ) % 7 + 1;
            else
                prev = -1;
            int curr = (chorale[j][i] + 4 ) % 7 + 1;
            int next = (chorale[j][i+1] + 4) % 7 + 1;
            int next2;
            if(i < 30)
                next2 = (chorale[j][i+2] + 4) % 7 + 1;
            else
                next2 = -1;

            if(curr == 4 && /*next != 3 &&*/ intervals[1] != 1 && j != 3)
                flag = 1;
            else if(curr == 7){ //Check movement around 7
                //Check 7-1 or 7-6
                if(next != 1 && next != 6)
                    flag = 1;
                else if(!(next == 6 && next2 == 5 && prev == 1) && next != 1) //Check 1-7-6-5
                    flag = 1;
            }

            if(flag > 0){
                fitness+=PUNISH_TENDENCY_TONES;
                if(last){
                    fputs("Beat ", fp);
                    fprintf(fp, "%d", i+1);
                    fputc('-', fp);
                    fprintf(fp, "%d", i+2);
                    fputs(": ", fp);
                    voiceToName(j, fp);
                    fputs(": Bad melodic movement: ", fp);
                    fprintf(fp, "%d", curr);
                    fputc('-', fp);
                    fprintf(fp, "%d", next);
                    fputc('\n', fp);
                    if(flag == 2) fputs("  leap error \n ", fp);
                }
            }
        }
    }//end for

    //Big Leaps and repeated leaps
    for(i = 0; i < 3; i++){
        int leaps = 0;
        for(k = 0; k < LENGTH - 1; k++){
            if(chorale[i][k] - chorale[i][k+1] > 4 || chorale[i][k] - chorale[i][k+1] < -4){
                fitness += PUNISH_BIG_LEAP;
                if(last){
                    fputs("Beat ", fp);
                    fprintf(fp, "%d", k+1);
                    fputc('-', fp);
                    fprintf(fp, "%d", k+2);
                    fputs(": ", fp);
                    voiceToName(i, fp);
                    fputs(": Too big of leap: ", fp);
                    fprintf(fp, "%d", chorale[i][k]);
                    fputc('-', fp);
                    fprintf(fp, "%d", chorale[i][k+1]);
                    fputc('\n', fp);
                }
            }

            if(chorale[i][k] - chorale[i][k+1] > 2 || chorale[i][k] - chorale[i][k+1] < -2)
                leaps++;
            else leaps = 0;
            if(leaps > 1){
                //fitness+=PUNISH_TOO_MANY_LEAPS;
                if(last){
                    fputs("Beat ", fp);
                    fprintf(fp, "%d", k+1);
                    fputs(": ", fp);
                    voiceToName(i, fp);
                    fputs(": Too many leaps\n", fp);
                }
            }
        }
    }

    //BIG loop for many errors
    for(i = 0; i < 3; i++){
        for(j = i+1; j < 4; j++){
            for(k = 0; k < LENGTH; k++){
                int Anote1 = chorale[i][k];
                int Anote2;
                if(k != 31)
                    Anote2 = chorale[i][k+1];
                else
                    Anote2 = -1;
                int Bnote1 = chorale[j][k];
                int Bnote2;
                if(k != 31)
                    Bnote2 = chorale[j][k+1];
                else
                    Bnote2 = -1;

                int move1 = Anote1-Anote2;
                int move2 = Bnote1-Bnote2;

                int int1 = Anote1-Bnote1;
                int int2 = Anote2-Bnote2;

                //Within Octave
                if(j-i == 1 && j != 3 && Anote1 - Bnote1 > 6){
                    fitness += PUNISH_WITHIN_OCTAVE;
                    if(last){
                        fputs("Beat ", fp);
                        fprintf(fp, "%d", k+1);
                        fputs(": Too far apart: ", fp);
                        voiceToName(i, fp);
                        fputc('-', fp);
                        voiceToName(j, fp);
                        fputc('\n', fp);
                    }
                }

                //Overlap
                if(j - i == 1 && Anote1 <= Bnote1){ //Adjacent voices
                    fitness += PUNISH_OVERLAP;
                    if(last){
                        fputs("Beat ", fp);
                        fprintf(fp, "%d", k+1);
                        fputs(": ", fp);
                        voiceToName(i, fp);
                        fputs(" crosses ", fp);
                        voiceToName(j, fp);
                        fprintf(fp, "%d", Anote1);
                        fputc(' ', fp);
                        fprintf(fp, "%d", Bnote1);
                        fputc('\n', fp);
                    }
                }

                //Direct 5ths and 8ves
                if(i==0 && j==3 && k < LENGTH - 1){//ensure soprano and bass comparison
                    if(move1 > 2){ //leap
                        if(move2 > 0 && move1 != move2){//Similar, not parallel, movement
                            if((int2 % 7 + 1 == 5 && Bnote2 % 7 != 2) || (int2 % 7 + 1 == 1 && Bnote1 != Bnote2)){
                                fitness += PUNISH_DIRECT_5_8;
                                if(last){
                                   fputs("Beat ", fp);
                                   fprintf(fp, "%d", k+1);
                                   fputc('-', fp);
                                   fprintf(fp, "%d", k+2);
                                   fputs(": Direct ", fp);
                                   if(int2 % 7 + 1 == 5)
                                        fputs("fifth between Soprano and Bass.\n", fp);
                                   else
                                        fputs("octave between Soprano and Bass.\n", fp);
                                }
                            }
                        }
                    }
                    if(move1 < -2){ //leap opposite direction
                        if(move2 < 0 && move1 != move2){//Similar, not parallel, movement
                            if((int2 % 7 + 1 == 5 && Bnote2 % 7 != 2) || (int2 % 7 + 1 == 1 && Bnote1 != Bnote2)){
                                fitness += PUNISH_DIRECT_5_8;
                                if(last){
                                   fputs("Beat ", fp);
                                   fprintf(fp, "%d", k+1);
                                   fputc('-', fp);
                                   fprintf(fp, "%d", k+2);
                                   fputs(": Direct ", fp);
                                   if(int2 % 7 + 1 == 5)
                                        fputs("fifth between Soprano and Bass.\n", fp);
                                   else
                                        fputs("octave between Soprano and Bass.\n", fp);
                                }
                            }
                        }
                    }
                }

                //Parallel 5ths and 8ves
                //Unequal 5ths
                if(move1 == move2 && Anote1 != Anote2){ //parallel, not same, motion
                    if(int1 % 7 + 1 == 5 && int2 % 7 + 1 == 5){ //Both 5ths
                        if(Bnote1 % 7 == 2 && Bnote2 % 7 != 2 && j == 3){ //d5 to P5 and in the bass voice
                            fitness += PUNISH_UNEQUAL_5;
                            if(last){
                                fputs("Beat ", fp);
                                fprintf(fp, "%d", k+1);
                                fputc('-', fp);
                                fprintf(fp, "%d", k+2);
                                fputs(": Unequal fifth between ", fp);
                                voiceToName(i, fp);
                                fputs(" and ", fp);
                                voiceToName(j, fp);
                                fputc('\n', fp);
                            }
                        }
                        else{
                            fitness += PUNISH_PARALLEL_5_8;
                            if(last){
                                fputs("Beat ", fp);
                                fprintf(fp, "%d", k+1);
                                fputc('-', fp);
                                fprintf(fp, "%d", k+2);
                                fputs(": Parallel fifth between ", fp);
                                voiceToName(i, fp);
                                fputs(" and ", fp);
                                voiceToName(j, fp);
                                fputc('\n', fp);
                            }

                        }
                    }
                    else if(int1 % 7 + 1 == 1 && int2 % 7 + 1 == 1 && Anote1 != Anote2 && Bnote1 != Bnote2){ //Both 8ves
                        fitness += PUNISH_PARALLEL_5_8;
                        if(last){
                            fputs("Beat ", fp);
                            fprintf(fp, "%d", k+1);
                            fputc('-', fp);
                            fprintf(fp, "%d", k+2);
                            fputs(": Parallel octave between ", fp);
                            voiceToName(i, fp);
                            fputs(" and ", fp);
                            voiceToName(j, fp);
                            fputc('\n', fp);
                        }
                    }
                }

                //Contrary 5ths and 8ves
                if(7-abs(move1)==abs(move2) && move1 * move2 < 0 && k < 31){ //Contrary inverted motion
                    if(int1 % 7 + 1 == 5 && int2 % 7 + 1 == 5 && Bnote1 % 7 != 2 && Bnote2 % 7 != 2){//Both P5
                        fitness += PUNISH_CONTRARY_5_8;
                        if(last){
                            fputs("Beat ", fp);
                            fprintf(fp, "%d", k+1);
                            fputc('-', fp);
                            fprintf(fp, "%d", k+2);
                            fputs(": Contrary fifth between ", fp);
                            voiceToName(i, fp);
                            fputs(" and ", fp);
                            voiceToName(j, fp);
                            fputc('\n', fp);
                        }
                    }
                    if(int1 % 7 + 1 == 1 && Anote1 != Anote2 && int2 % 7 + 1 == 1 && Bnote1 != Bnote2){//Both P8
                        fitness += PUNISH_CONTRARY_5_8;
                        if(last){
                            fputs("Beat ", fp);
                            fprintf(fp, "%d", k+1);
                            fputc('-', fp);
                            fprintf(fp, "%d", k+2);
                            fputs(": Contrary octave between ", fp);
                            voiceToName(i, fp);
                            fputs(" and ", fp);
                            voiceToName(j, fp);
                            fputc('\n', fp);
                        }
                    }
                }
            }
            leaps = 0;
        }
    }

/*    //NORMALIZE FITNESS TO 0-1
    fitness = fitness / 99;

    if(last){
        fputs("\nFitness: ", fp);
        fprintf(fp, "%1.1f", fitness);
        close(fp);
    }*/

    int beat = 0;
    int found1 = 0, found2 = 0, found3 = 0, found4 = 0, found5 = 0, found6 = 0, found7 = 0;
    for(; beat < LENGTH; beat++){
        if(beat > 0 && chord_prog[beat] == chord_prog[beat-1]) fitness -= 1.0;
        switch(chord_prog[beat]){
            case 1: found1++; break;
            case 2: found2++; break;
            case 3: found3++; break;
            case 4: found4++; break;
            case 5: found5++; break;
            case 6: found6++; break;
            case 7: found7++; break;
        }
    }

    int num1 = (LENGTH / 3 > 1)? LENGTH / 3: -1;
    int num4 = (LENGTH / 6 > 1)? LENGTH / 6: -1;
    int num6 = (LENGTH / 6 > 1)? LENGTH / 6: -1;

	    if(found1 == 0 || (found1 > num1 && num1 != -1))
        fitness -= 1.0;
    if(found2 == 0)// || found2 < 4)
        fitness -= 1.0;
    if(found3 == 0)// || found3 > 3)
        fitness -= 1.0;
    if(found4 == 0 || (found4 > num4 && num4 != -1))
        fitness -= 1.0;
    if(found5 == 0)// || found5 < 5)
        fitness -= 1.0;
    if(found6 == 0 || (found6 > num6 && num6 != -1))
        fitness -= 1.0;
    if(found7 == 0)// || found7 < 5)
        fitness -= 1.0;

    if((chorale[3][LENGTH-1] + 4) % 7 + 1 != 1)
        fitness -= 1.0;

    return fitness;
}

int main(){
    srand(time(NULL));

    //Allocate memory for the chorales
    int ***chorales = (int ***)malloc(POPULATION_SIZE * sizeof(int **));
    int *chordProg = (int *)malloc(LENGTH * sizeof(int));
    int i,j,k;
    for(i = 0; i < POPULATION_SIZE; i++){
        chorales[i] = (int **)malloc(4 * sizeof(int *));
        for(j = 0; j < 4; j++){
            chorales[i][j] = (int *)malloc(LENGTH * sizeof(int));
        }
    }

    double * fitness = (double *)malloc(POPULATION_SIZE * sizeof(double));

    //Fill with random notes and find fitness
    for(k = 0; k < POPULATION_SIZE; k++){
        create_random_chorale(chorales[k], chordProg);
        fitness[k] = findFitness(chorales[k], chordProg,0);
    }

    int generation = 0;
    int repeats = 0;
    double repeat_fitness = -500;

    //Begin evolution
    while(generation < NUM_GENERATIONS && fitness[0] < -0.5){

        //Find new fitness
        for(k = POPULATION_SIZE * .1; k < POPULATION_SIZE; k++){
            fitness[k] = findFitness(chorales[k], chordProg, 0);
        }

        //Sort by highest fitness to lowest fitness
        select_sort(chorales, fitness, POPULATION_SIZE);

        //Check for plateaus in the fitness function
        if(repeat_fitness == fitness[0]) repeats++;
        else{
            repeat_fitness = fitness[0];
            repeats = 0;
        }

        //Code for statistics
        double fitness_tot = 0.0;

        for(i = 0; i < POPULATION_SIZE; i++) fitness_tot += fitness[i];

        fitness_tot /= POPULATION_SIZE;

        if(generation % 1000 == 0) fitness[0] = findFitness(chorales[0], chordProg, 0);

        //printf("Generation: %i; Fitness: %f; Avg. Fitness: %f; Lowest Fitness: %f", generation, fitness[0], fitness_tot, fitness[POPULATION_SIZE-1]);
	printf("%i\t%f\t%f\t%f", generation, fitness[0], fitness_tot, fitness[POPULATION_SIZE-1]);
        printf("\n");
        generate(chorales, fitness);
        generation++;
    }

    //Setup code for lilypond file creation
    findFitness(chorales[0], chordProg, 1);
    printf("Chord Progression: ");

    for(i = 0; i < LENGTH; i++){
        printf("%i ", chordProg[i]);
    }

    //Create file
    printf("\nMAKING FILE\n");

    makeFile(chorales[0], chordProg);

    printf("FINISHED \"chorale.ly\" FILE\n");
}

void add_chord(int **chorale, int *chords, int position){
    //printf("add_chord\n");
    chorale[0][position] = chords[3];
    chorale[1][position] = chords[2];
    chorale[2][position] = chords[1];
    chorale[3][position] = chords[0];
}

int chord_type(int position){
    //printf("chord_type\n");
    if(position >= 0 && position < 24)
        return 6;
    else if(position >= 24 && position < 34)
        return 7;
    else if(position >= 34 && position < 62)
        return 1;
    else if(position >= 62 && position < 82)
        return 2;
    else if(position >= 82 && position < 92)
        return 3;
    else if(position >= 92 && position < 103)
        return 4;
    else if(position >= 103 && position < 113)
        return 5;
    else return -1;
}

void create_random_chorale(int **chorale, int *chords){
    int beat;
    for(beat = 0; beat < LENGTH; beat++){
        int random_chord = rand() % NUM_OF_CHORDS;

        //Add chord number to chord progression
        chords[beat] = chord_type(random_chord);

        //Add chord to chorale
        add_chord(chorale, CW[random_chord], beat);
    }
}

int randNote(int voice){
    switch(voice){
        case 0: return rand()%12 + 10; break;//Soprano
        case 1: return rand()%12 + 7; break; //Alto
        case 2: return rand()%12 + 3; break; //Tenor
        case 3: return rand()%12 + 0; break; //Bass
    }
}

void select_sort(int *** chorales, double * fitness, int size){
    int i, j, pos;
    for(i = 0; i < size-1; i++){
        pos = i;
        for(j = i + 1; j < size; j++){
            if((fitness)[pos] < (fitness)[j])
                pos = j;
        }
        if(pos != i){
            int ** temp_ipp;
            double temp_d;
            temp_d = (fitness)[i];
            temp_ipp = (chorales)[i];
            (fitness)[i] = (fitness)[pos];
            (chorales)[i] = (chorales)[pos];
            (fitness)[pos] = temp_d;
            (chorales)[pos] = temp_ipp;
        }
    }
}

