# Lab 01 — Cancela Automática (ESP32 / ESP-IDF)

Passo a passo para implementar, compilar e simular este projeto.

## 1. Estrutura de pastas

```
lab01_cancela/
├── CMakeLists.txt
├── diagram.json          <- circuito para o Wokwi
├── wokwi.toml
├── main/
│   ├── CMakeLists.txt
│   └── main.c             <- só inicia e chama app_run_once()
└── components/
    ├── hardware/           <- única camada que toca driver/gpio.h
    │   ├── CMakeLists.txt
    │   ├── hardware.c
    │   └── include/hardware.h
    ├── services/           <- sensores/atuadores "de domínio"
    │   ├── CMakeLists.txt
    │   ├── services.c
    │   └── include/services.h
    └── app/                <- máquina de estados (regras da cancela)
        ├── CMakeLists.txt
        ├── app.c
        └── include/app.h
```

## 2. Pré-requisitos

- ESP-IDF instalado (v5.x) — `get_started` no site do ESP-IDF, ou
- Extensão **Wokwi Simulator** no VS Code (mais simples, não precisa de placa física).

## 3. Passo a passo

1. **Criar o projeto**: copie esta pasta `lab01_cancela` inteira para o seu
   workspace (ou gere um projeto vazio com `idf.py create-project` e
   substitua `main/` e crie a pasta `components/` com os arquivos acima).
2. **Configurar o target**:
   ```
   idf.py set-target esp32
   ```
3. **Compilar**:
   ```
   idf.py build
   ```
4. **Testar cada entrada/saída isoladamente (Parte 1 do roteiro)**
   Antes de integrar tudo, é recomendável criar um `main.c` temporário que
   apenas chama `hardware_init()` e faz *polling* de cada `hardware_read_raw()`,
   acendendo um LED de teste — confirme:
   - que a chave com pull-down lê `0` em repouso e `1` quando pressionada;
   - que a chave com pull-up lê `1` em repouso e `0` quando pressionada;
   - que cada LED acende/apaga corretamente com `hardware_write_raw()`.
   Depois de validado, volte para o `main.c` definitivo (o que chama `app_init`/`app_run_once`).
5. **Simular no Wokwi**:
   - Abra a pasta no VS Code com a extensão Wokwi instalada;
   - O arquivo `diagram.json` já contém 5 botões (chaves) e 4 LEDs
     ligados às GPIOs corretas;
   - Pressione **F1 → "Wokwi: Start Simulator"** (ou clique no botão de play).
6. **Executar os testes da Parte 5** (veja tabela de testes no relatório):
   - ciclo normal de entrada;
   - veículo detectado durante o fechamento;
   - emergência durante abertura;
   - emergência durante fechamento.
   Acompanhe os LEDs na simulação e o log serial (`ESP_LOGI`) que imprime
   cada transição de estado.
7. **(Opcional) Gravar em placa física**:
   ```
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

## 4. Regra de ouro da arquitetura

- `components/app/app.c` **não pode** incluir `driver/gpio.h` nem `hardware.h`.
- `components/services/services.c` é o único lugar que decide se um sensor
  é "ativo em alto" ou "ativo em baixo".
- `components/hardware/hardware.c` é o único lugar que sabe os números das GPIOs.

Se, ao revisar o código, `app.c` conhecer um número de GPIO, a arquitetura
foi violada e o roteiro (Parte 3) não foi atendido.
