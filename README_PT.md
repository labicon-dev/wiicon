# WiiCon Remote

> Um controle de movimento sem fio baseado em ESP32 para aplicações criativas, inspirado na arquitetura do Wii Remote.

**⚠️ Este projeto está em desenvolvimento ativo como parte de um projeto acadêmico na UFBA.**

## Visão Geral

**WiiCon** é um controlador de movimento de código aberto construído utilizando o microcontrolador **ESP32-C6 Super Mini** e a IMU **BMI160 de 6 eixos** (acelerômetro + giroscópio).

Ele processa dados de fusão de sensores no próprio dispositivo usando um filtro Madgwick AHRS e transmite a orientação (ângulos de Euler) ou dados brutos via WiFi usando o protocolo **OSC (Open Sound Control)**, tornando-se uma solução ideal para:

- Instalações de arte interativa
- Síntese sonora controlada por gestos
- Programação criativa (Processing, TouchDesigner, Max/MSP, Pure Data)
- Prototipagem de captura de movimento
- Desenvolvimento de jogos

## Funcionalidades

- **Modos de Dados Duplos:** Alterne entre Orientação Processada (Roll, Pitch, Yaw) e Dados Brutos do sensor.
- **Gerenciador WiFi:** Configure a rede WiFi e o IP de destino do OSC usando uma interface web.
- **Deep Sleep:** Entra em sono profundo quando o botão é pressionado por 3 segundos.
- **Transmissão OSC:** Envio via UDP de baixa latência dos dados do sensor para um IP de destino e porta configuráveis.
- **Feedback Visual:** Indicadores de status LED RGB para startup, conexão, atividade e erros.
- **Auto-Calibração:** Correção automática de viés do giroscópio na inicialização ou via comando.

## Hardware e Conexões

### Componentes

| Componente           | Descrição                                       |
| :------------------- | :---------------------------------------------- |
| **Microcontrolador** | ESP32-C6 Super Mini (ou placa ESP32 compatível) |
| **Sensor Inercial**  | Bosch BMI160 6-eixos (acelerômetro/giroscópio)  |
| **Feedback**         | LED RGB para indicação de status                |
| **Entrada**          | Botão Tactil                                    |

### Diagrama de Ligação

Baseado no arquivo `config.h` padrão:

| Component Pin    | ESP32 GPIO | Note                           |
| :--------------- | :--------- | :----------------------------- |
| **BMI160 SDA**   | `GPIO 4`   | I2C Data                       |
| **BMI160 SCL**   | `GPIO 2`   | I2C Clock                      |
| **Botão**        | `GPIO 3`   | Soldado entre o GPIO 3 e o GND |
| **LED Vermelho** | `GPIO 18`  | Resistor de 220Ω               |
| **LED Verde**    | `GPIO 19`  | Resistor de 220Ω               |
| **LED Azul**     | `GPIO 20`  | Resistor de 220Ω               |

## Controles e Interface

O dispositivo possui um único **botão multifuncional** para controlar o dispositivo sem necessidade de um computador.

| Ação             | Função             | Descrição                                                                        |
| :--------------- | :----------------- | :------------------------------------------------------------------------------- |
| **1 Clique**     | **Alternar Modo**  | Alterna o envio entre **Dados Processados** (Euler) e **Dados Brutos** (Raw).    |
| **2 Cliques**    | **Recalibrar**     | Recalibra o Giroscópio. **Mantenha o dispositivo imóvel** durante o LED amarelo. |
| **3 Cliques**    | **Resetar WiFi**   | Limpa as credenciais WiFi e reinicia em Modo AP para reconfiguração.             |
| **Segurar (3s)** | **Ligar/Desligar** | Entra ou sai do modo **Deep Sleep** (Sono Profundo).                             |

## Indicadores de Status (LED)

O LED RGB fornece feedback em tempo real sobre o estado do sistema:

| Cor / Padrão                  | Significado                                                     |
| :---------------------------- | :-------------------------------------------------------------- |
| ⚪ **Branco (Fixo 1s)**        | **Inicialização.** O sistema está iniciando.                    |
| 🟡 **Amarelo (Piscando)**      | **Busca WiFi.** Tentando conectar à rede ou Modo AP ativo.      |
| 🟢 **Verde (Fixo 2s)**         | **Sucesso.** WiFi conectado e sensores calibrados.              |
| 🔵 **Azul (Flash Breve)**      | **Atividade.** Pisca a cada 2s indicando transmissão OSC ativa. |
| 🔴 **Vermelho (Pisca Rápido)** | **Erro de Hardware.** Sensor BMI160 não detectado.              |
| 🟣 **Magenta (Pisca Lento)**   | **Erro de Rede.** Timeout na conexão.                           |

## Protocolo OSC

O dispositivo transmite dados via UDP para o IP configurado no portal (porta padrão: `9000`).

### Endereços

- **Ângulos de Euler:** `/wiicon/euler`
  - Argumentos: `float roll`, `float pitch`, `float yaw` (Graus)
- **Acelerômetro Bruto:** `/wiicon/accel` (Apenas no Modo Raw)
  - Argumentos: `float x`, `float y`, `float z` (força g)
- **Giroscópio Bruto:** `/wiicon/gyro` (Apenas no Modo Raw)
  - Argumentos: `float x`, `float y`, `float z` (graus/s)

## Instalação e Configuração

1. **Clone o repositório:** `git clone https://github.com/labicon-dev/wiicon.git`
2. **Abra no Arduino IDE** (ou PlatformIO).
3. **Configuração:** Edite o arquivo `config.h` se a sua pinagem for diferente.
4. **Upload:** Selecione a placa **ESP32C6 Dev Module** e envie o código.
5. **Primeiro Uso:**
    - O LED piscará em Amarelo.
    - Conecte-se à rede WiFi **"WiiCon Setup"**.
    - Um portal abrirá automaticamente (ou acesse `192.168.4.1`).
    - Insira o SSID/Senha da sua rede e as configurações avançadas se necessário.

## Autores

- **Breno Paz** — <brenopaz@ufba.br>
- **Eduardo Monteiro** — <eduardo.monteiro@ufba.br>
- **Laura Marques** — <laura.marques@ufba.br>
- **Vitor Rizzato** — <vitormagalhaes@ufba.br>

### Código de Terceiros

- **Driver BMI160:** Derivado da biblioteca [Intel BMI160-Arduino](https://github.com/arduino/ArduinoCore-arc32/blob/master/libraries/CurieIMU/src/BMI160.h) e [Jeff Rowberg's I2Cdev library](https://github.com/jrowberg/i2cdevlib).
- **Madgwick AHRS:** Implementação baseada no algoritmo de [Sebastian Madgwick](https://x-io.co.uk/downloads/madgwick_internal_report.pdf).

## Licença

Este projeto foi desenvolvido na **Universidade Federal da Bahia (UFBA)** como parte do projeto final da disciplina **Dispositivos Tecnológicos Práticos II**.

Este projeto está licenciado sob a **Licença MIT**. Veja o arquivo `LICENSE` para mais detalhes.
