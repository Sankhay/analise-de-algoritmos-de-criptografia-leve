#ifndef METRICS_UTIL_H
#define METRICS_UTIL_H

#include <stddef.h>   
#include <stdint.h>   
#include <stdlib.h>    

#ifdef ARDUINO
#include <Arduino.h>
#include <SD.h>      
#endif

// --- Assinaturas das Funções ---

/**
 * @brief Converte um bloco de bytes para uma string hexadecimal (malloc'd).
 *
 * @param src Ponteiro para os dados de origem.
 * @param len Número de bytes a serem convertidos.
 * @return Um ponteiro para a string hexadecimal alocada dinamicamente,
 * ou NULL em caso de falha na alocação. O chamador é responsável por liberar a memória.
 */
char* bytesToHex(const void* src, size_t len);

/**
 * @brief Exibe métricas de desempenho no Serial e salva em um arquivo SD em formato CSV.
 *
 * @param dataFile Objeto File aberto onde as métricas serão salvas (CSV).
 * @param metric Estrutura PerformanceMetrics contendo os dados.
 */
// NOTA: O tipo 'File' pressupõe que <SD.h> ou similar foi incluído.
void displayAndSaveMetric(File& dataFile, const PerformanceMetrics& metric);

/**
 * @brief Lê e exibe o conteúdo completo de um arquivo do SD no Serial.
 *
 * @param filename Nome do arquivo no cartão SD.
 */
void readAndDisplayFile(const char* filename);

#endif // METRICS_UTIL_H