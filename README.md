# 🛸 Space Connect — Sistema IoT de Monitoramento de Cápsula Espacial

> **Global Solution 2026 — FIAP**  
> Disciplina: Computer Organization and Architecture  
> Integrantes: Arthur Primo Brandão (RM 573572) · Vinicius de Oliveira Coppola (RM 571699)  


---

## 📌 Contexto do Projeto

A iniciativa *Global Solution 2026* propôs a temática **"Space Connect"**, com foco no desenvolvimento de soluções para o monitoramento contínuo de variáveis críticas em missões espaciais — temperatura, luminosidade e vibração/impacto. O sistema desenvolvido é uma prova de conceito de um **módulo de telemetria e segurança espacial**, capaz de coletar, processar e reagir em tempo real às condições internas e operacionais de um módulo espacial tripulado.

O código implementado roda em um **Arduino Uno R3 (ATmega328P)** e integra sensores analógicos, atuadores cinéticos (servos), sinalizadores visuais (LEDs), sinal sonoro (buzzer) e interface homem-máquina (display LCD I2C).

---

## 📦 Bibliotecas Utilizadas

| Biblioteca | Função |
|---|---|
| `Servo.h` | Controle dos dois micro servos motores |
| `Wire.h` | Comunicação I2C (necessária para o LCD) |
| `LiquidCrystal_I2C.h` | Controle do display LCD 16x2 via I2C |

---

## 🔌 Hardware e Pinagem

| Componente | Pino | Modo |
|---|---|---|
| Sensor de Força — FSR (Transdutor de Esforço Mecânico) | A0 | INPUT analógico |
| Sensor de Temperatura — TMP36 (Transdutor Térmico) | A1 | INPUT analógico |
| Fotoresistor — LDR (Transdutor de Luminosidade) | A2 | INPUT analógico |
| LED — Alerta de Impacto | 2 | OUTPUT digital |
| LED — Alerta de Alta Temperatura | 3 | OUTPUT digital |
| LED — Alerta de Brilho Intenso | 4 | OUTPUT digital |
| Micro Servo Motor 1 | 5 | OUTPUT (PWM) |
| Micro Servo Motor 2 | 6 | OUTPUT (PWM) |
| Buzzer Piezoelétrico | 7 | OUTPUT digital |
| Display LCD 16x2 I2C (endereço 0x27) | SDA / SCL | I2C |

---

## ⚙️ Funcionamento do Código

### Inicialização (`setup`)

- Inicia a comunicação serial a 9600 bps.
- Configura os pinos dos LEDs e do buzzer como saídas.
- Inicializa o LCD, exibe `"LCD acesa!"` por 2 segundos como confirmação de boot e limpa a tela.
- Associa os servomotores aos pinos 5 e 6.

---

### Loop Principal (`loop`)

A cada ciclo, o firmware executa três etapas em sequência:

#### 1. Leitura dos Sensores

| Sensor | Pino | Processamento |
|---|---|---|
| FSR (Força) | A0 | Leitura bruta de 0 a 1023 |
| TMP36 (Temperatura) | A1 | Conversão para °C via fórmula do sensor |
| LDR (Luminosidade) | A2 | Leitura bruta de 0 a 1023 |

**Fórmula de conversão do TMP36:**
```
Tensão (V)   = (leitura × 5.0) / 1024.0
Temperatura  = (Tensão - 0.5) × 100
```

---

#### 2. Verificação de Alertas — Regime de Interrupção

Os alertas são avaliados em cascata (`if / else if`). Quando qualquer variável ultrapassa seu *threshold* de segurança, o sistema entra em **modo de alerta**, executando automaticamente as seguintes ações de salvaguarda:

- Exibição prioritária no LCD identificando o subsistema em falha;
- Ativação do LED correspondente ao sensor que originou o evento;
- Emissão de sinal sonoro de emergência via buzzer (1000 Hz por 1 segundo);
- **Travamento imediato dos servos** (eles param de se mover enquanto há alerta), mitigando riscos de danos mecânicos secundários.

| Condição | Limiar | LED | Mensagem no LCD |
|---|---|---|---|
| Impacto / Estresse Mecânico | `FSR ≥ 850` | Pino 2 | `ALERTA ALERTA` / `IMPACTO` |
| Alta Temperatura | `Temperatura ≥ 70 °C` | Pino 3 | `ALTA TEMPERATURA` / `PERIGO` |
| Radiação Luminosa Intensa | `LDR ≥ 512` | Pino 4 | `Brilho intenso` / `PERIGO` |

> **Nota:** Por usar `else if`, apenas o primeiro alerta ativo é exibido por ciclo. Caso dois eventos simultâneos ocorram, o de maior prioridade na ordem do código (impacto > temperatura > luz) prevalece.

---

#### 3. Modo Normal — Regime de Operação Nominal

Quando todas as variáveis estão dentro dos limites seguros:

- Todos os LEDs de alerta são desligados (`LOW`).
- O LCD exibe as leituras em tempo real:
  - **Linha 0 (esquerda):** Força → `F:XXXn`
  - **Linha 0 (direita, coluna 9):** Luminosidade → `L:XXX`
  - **Linha 1:** Temperatura → `Temp: XX.X C`
- Os dois micro servos executam um **ciclo contínuo de demonstração**, simulando subsistemas cinéticos ativos da espaçonave:
  - Movem-se para **0°** → aguardam 700 ms
  - Movem-se para **90°** → aguardam 700 ms

---

## 🔢 Limiares de Segurança

| Sensor | Limiar de Alerta | Base |
|---|---|---|
| FSR (Força) | ≥ 850 (escala 0–1023) | Detecção de impacto ou estresse estrutural elevado |
| TMP36 (Temperatura) | ≥ 70 °C | Limite térmico de segurança da cabine |
| LDR (Luminosidade) | ≥ 512 (escala 0–1023) | Acima de ~50% do range — indicativo de radiação intensa |

---

## 📋 Pré-requisitos

- Arduino IDE (1.8+ ou 2.x)
- Bibliotecas (instalar via **Gerenciador de Bibliotecas**):
  - `Servo` — já inclusa no Arduino IDE
  - `LiquidCrystal I2C` — por Frank de Brabander

---

## 🚀 Como Usar

1. Monte o circuito conforme a tabela de pinagem acima.
2. Abra o arquivo `.ino` na Arduino IDE.
3. Selecione a placa **Arduino Uno** e a porta serial correta.
4. Faça o upload do código.
5. O LCD exibirá `"LCD acesa!"` confirmando a inicialização.
6. Acesse o **Monitor Serial** (9600 bps) para acompanhar as leituras, se necessário.

---
