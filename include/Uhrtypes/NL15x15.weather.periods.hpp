#pragma once

#include "Uhrtype.hpp"

/*
 *              Layout Front
 *                  COL
 *       4 3 2 1 X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - - - - - - 
 *  0  | H E T K I S A V I J F N O N Z
 *  1  | T I E N A M C D V O O R Z N A
 *  2  | O V E R T I K W A R T H G A S
 *  3  | G W S E H A L F B H X O V E R
 *  4  | N A V O O R Z E E N O T W E E
 *  5  | D R I E L V I E R D E V I J F
 *  6  | F E R Z E S F O Z E V E N S A
 *  7  | A C H T D H N V I L N E G E N
 *  8  | T I E N E L F S L T W A A L F
 *  9  | O D M U U R S E B I N B D E T
 *  X  | N A C H T X V I O C H T E N D
 *  1  | N A M I D D A G S D A V O N D
 *  2  | V O O R M I D D A G O F E N R
 *  3  | A L Z O N N I G B E W O L K T
 *  4  | M E T A R E G E N S N E E U W
 *
 */

class Nl15x15WeatherPeriods_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::NL;
    };

    virtual const uint8_t rowsWordMatrix() override { return 15; };

    virtual const uint8_t colsWordMatrix() override { return 15; };

    virtual const bool hasWeatherLayout() override { return true; }

    virtual const bool hasZwanzig() override { return false; }

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 12, 14);
            setFrontMatrixWord(0, 9, 10); 
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 4, 8);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 4, 7);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 11, 14);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::nach:
            setFrontMatrixWord(2, 11, 14);
            break;

        case FrontWord::v_nach:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::vor:
            setFrontMatrixWord(1, 3, 6);
            break;

        case FrontWord::v_vor:
            setFrontMatrixWord(4, 9, 12);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 9, 11);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(4, 5, 7);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(5, 11, 14);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(5, 6, 9);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(6, 9, 11);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(6, 2, 6);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(7, 11, 14);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(7, 0, 4);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 11, 14);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(8, 8, 10);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(8, 0, 5);
            break;

        //------------------------------------------------------------------------------
        // Weather
        //------------------------------------------------------------------------------

        case FrontWord::w_schnee:
            setFrontMatrixWord(14, 12, 24);
            setFrontMatrixWord(14, 0, 5);
            break;

        case FrontWord::w_klar:
            setFrontMatrixWord(12, 1, 2);
            setFrontMatrixWord(13, 7, 12);
            break;

        case FrontWord::w_regen:
            setFrontMatrixWord(14, 12, 24);
            setFrontMatrixWord(14, 6, 10);
            break;

        case FrontWord::w_wolken:
            setFrontMatrixWord(12, 1, 2);
            setFrontMatrixWord(13, 0, 6);
            break;

        case FrontWord::w_gewitter:
            setFrontMatrixWord(14, 12, 24);
            setFrontMatrixWord(14, 6, 10);
            break;

        //------------------------------------------------------------------------------
        // Day Period
        //------------------------------------------------------------------------------

        case FrontWord::w_nacht:
            setFrontMatrixWord(9, 4, 5);
            setFrontMatrixWord(9, 1, 2);
            setFrontMatrixWord(10, 10, 14);
            break;

        case FrontWord::w_mittag:
            setFrontMatrixWord(9, 4, 5);
            setFrontMatrixWord(9, 1, 2);
            setFrontMatrixWord(11, 7, 12);
            break;

        case FrontWord::w_abend:
            setFrontMatrixWord(9, 4, 5);
            setFrontMatrixWord(9, 1, 2);
            setFrontMatrixWord(11, 0, 4);
            break;

        case FrontWord::w_morgen:
            setFrontMatrixWord(9, 4, 5);
            setFrontMatrixWord(9, 1, 2);
            setFrontMatrixWord(10, 0, 6);
            break;

        default:
            break;
        }
    };
};

Nl15x15WeatherPeriods_t _nl15x15WeatherPeriods;