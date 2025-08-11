/*
 * EykVoice - Sistema de Text-to-Speech para SA-MP
 * Versão: 1.0
 * Autor: Marinho
 * 
 * Funções principais:
 * - SpeakText(playerid, const text[]) - Reproduz TTS para jogador específico
 * - SpeakToAll(const text[]) - Reproduz TTS para todos os jogadores
 * - StopPlayerTTS(playerid) - Para reprodução TTS atual do jogador
 * - StopTTSForAll() - Para reprodução TTS para todos os jogadores
 * 
 * Códigos de retorno:
 * - TTS_SUCCESS (1) - Operação realizada com sucesso
 * - TTS_ERROR_PLAYER_NOT_CONNECTED (0) - Jogador não conectado
 * - TTS_ERROR_INVALID_TEXT (-1) - Texto inválido ou vazio
 * - TTS_ERROR_TEXT_TOO_LONG (-2) - Texto muito longo
 * 
 */

#if defined _GoogleTTS_included
	#endinput
#endif
#define _GoogleTTS_included

// Configuration
#if !defined MAX_TTS_TEXT_LENGTH
    #define MAX_TTS_TEXT_LENGTH 128
#endif

#if !defined TTS_LANGUAGE
    #define TTS_LANGUAGE "pt-br"
#endif

// Return codes
#define TTS_SUCCESS 1
#define TTS_ERROR_PLAYER_NOT_CONNECTED 0
#define TTS_ERROR_INVALID_TEXT -1
#define TTS_ERROR_TEXT_TOO_LONG -2


stock SpeakText(playerid, const text[]) {
    if(!IsPlayerConnected(playerid)) {
        return TTS_ERROR_PLAYER_NOT_CONNECTED;
    }
    
    new textLen = strlen(text);
    if(textLen == 0) {
        return TTS_ERROR_INVALID_TEXT;
    }
    
    if(textLen > MAX_TTS_TEXT_LENGTH) {
        return TTS_ERROR_TEXT_TOO_LONG;
    }
    
    static encodedText[MAX_TTS_TEXT_LENGTH + 64];
    UrlEncode(text, encodedText, sizeof(encodedText));
    
    static audioUrl[300];
    format(audioUrl, sizeof(audioUrl), "https://translate.google.com/translate_tts?ie=UTF-8&tl=%s&client=tw-ob&q=%s", TTS_LANGUAGE, encodedText);
    
    PlayAudioStreamForPlayer(playerid, audioUrl, 0.0, 0.0, 0.0, 1.0, 0);
    
    return TTS_SUCCESS;
}

stock SpeakToAll(const text[]) {
    new count = 0;
    for(new i = 0; i < MAX_PLAYERS; i++) {
        if(SpeakText(i, text)) {
            count++;
        }
    }
    return count;
}

stock StopPlayerTTS(playerid) {
    if(!IsPlayerConnected(playerid)) {
        return TTS_ERROR_PLAYER_NOT_CONNECTED;
    }
    
    StopAudioStreamForPlayer(playerid);
    return TTS_SUCCESS;
}

stock StopTTSForAll() {
    new count = 0;
    for(new i = 0; i < MAX_PLAYERS; i++) {
        if(IsPlayerConnected(i)) {
            StopAudioStreamForPlayer(i);
            count++;
        }
    }
    return count;
}

static stock UrlEncode(const input[], output[], maxlen) {
    static const hex[] = "0123456789ABCDEF";
    new pos = 0, len = strlen(input);
    
    for(new i = 0; i < len && pos < maxlen - 3; i++) {
        new ch = input[i];
        
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || 
           (ch >= '0' && ch <= '9') || ch == '-' || ch == '_' || 
           ch == '.' || ch == '~') {
            output[pos++] = ch;
        } else if(ch == ' ') {
            output[pos++] = '+';
        } else {
            if(pos < maxlen - 3) {
                output[pos++] = '%';
                output[pos++] = hex[(ch >> 4) & 0x0F];
                output[pos++] = hex[ch & 0x0F];
            }
        }
    }
    
    output[pos] = '\0';
}


