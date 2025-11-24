#include <SD.h>
#include "structs/structs.h"

char* bytesToHex(const void* src, size_t len) {
    char* dest = (char*)malloc(len * 2 + 1);
    if (dest == NULL) return NULL; 

    const uint8_t* byteSrc = (const uint8_t*)src;
    
    for (size_t i = 0; i < len; i++) {
        sprintf(&dest[i * 2], "%02X", byteSrc[i]);
    }
    dest[len * 2] = '\0';

    return dest;
}

void displayAndSaveMetric(File& dataFile, const PerformanceMetrics& metric) {
    unsigned long totalExecutionTime = metric.endTime - metric.startTime;
    unsigned long encryptionTime = metric.algorithmReturn.encryptionTime - metric.startTime;
    unsigned long decryptionTime = metric.endTime - metric.algorithmReturn.encryptionTime;
    const char* successString = metric.algorithmReturn.success ? "True" : "False";

    char* hexData = NULL;
    if (metric.algorithmReturn.encryptedData != NULL && metric.algorithm.encryptedDataSize > 0) {
        hexData = bytesToHex(metric.algorithmReturn.encryptedData, metric.algorithm.encryptedDataSize);
    } else {
        hexData = strdup("N/A");
    }

    char* plaintextHex = NULL;
    if (metric.plaintext != NULL && metric.algorithm.plainTextSize > 0) {
        plaintextHex = bytesToHex(metric.plaintext, metric.algorithm.plainTextSize);
    } else {
        plaintextHex = strdup("N/A");
    }

    char* keyHex = NULL;
    if (metric.key != NULL && metric.algorithm.keySize > 0) {
        keyHex = bytesToHex(metric.key, metric.algorithm.keySize);
    } else {
        keyHex = strdup("N/A");
    }
 
    char serialBuffer[512]; 
    char csvBuffer[512]; 

    sprintf(serialBuffer, "%-15s| %-12lu | %-12lu | %-12lu | %-7s | %-15s | %-15s | %s",
            metric.algorithm.algorithName,
            encryptionTime,
            decryptionTime,
            totalExecutionTime,
            successString,
            plaintextHex,      // NEW: Plaintext Hex
            keyHex,            // NEW: Key Hex
            hexData);          // Encrypted Data Hex

    // Saída CSV: Adicionando colunas (Note: Removed hexDataSize which was unused in final display)
    sprintf(csvBuffer, "%s,%lu,%lu,%lu,%s,%s,%s,%s",
            metric.algorithm.algorithName,
            encryptionTime,
            decryptionTime,
            totalExecutionTime,
            successString,
            plaintextHex,
            keyHex,
            hexData);

    Serial.println(serialBuffer);
    
    if (dataFile) {
        dataFile.println(csvBuffer);
    }

    if (hexData != NULL) {
        free(hexData);
    }
    if (plaintextHex != NULL) {
        free(plaintextHex);
    }
    if (keyHex != NULL) {
        free(keyHex);
    }

    if (metric.algorithmReturn.encryptedData != NULL) {
        free(metric.algorithmReturn.encryptedData);
    }

}

void readAndDisplayFile(const char* filename) {
    Serial.print("\n--- Conteúdo Completo do Arquivo SD (");
    Serial.print(filename);
    Serial.println(") ---");
    
    File dataFile = SD.open(filename);

    if (dataFile) {
        while (dataFile.available()) {
            Serial.write(dataFile.read());
        }
        
        dataFile.close();
        
        Serial.println("\n--- Fim da Leitura do Arquivo ---");
    } else {
        Serial.print("Erro ao abrir ");
        Serial.print(filename);
        Serial.println(" para leitura!");
    }
}
