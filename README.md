# Iterated Prisoner's Dilemma

A configurable C++ simulation framework for running and analysing Iterated Prisoner's Dilemma tournaments.

The project was developed to investigate the behaviour of different strategies under various conditions and repeated iterations. It supports configurable round-robin tournaments as well as evolutionary population simulations.

## The Prisoner's Dilemma
The prisoner's dilemma is a game-theory scenario in which two agents independently choose whether to cooperate or defect. Each combination of choices produces a payoff:
- Reward (R): both agents cooperate
- Temptation (T): one agent defects and the other cooperates
- Punishment (P): both agents defect
- Sucker (S): one agent cooperates while the other defects

In the iterated prisoner's dilemma, the same agents interact repeatedly. This allows them to react to previous rounds, build cooperation, retaliate against defection or attempt to exploit predictable opponents.

## Technologies
- C++20
- CMake
- Object-oriented programming
- C++ smart pointers
- Command-line interface
- GoogleTest (for automated testing)

## Overview

The simulator runs repeated prisoner's dilemma matches between configurable strategies and aggregates their performance across tournaments.

The engine supports:
- Double round-robin tournaments
- Configurable rounds and repeats
- Noise
- Configurable payoff values
- Deterministic random seeds
- Strategy selection through the command line
- Text, CSV and JSON output
- Mean payoff, standard deviation and 95% confidence intervals
- Saving and loading experiment configurations
- Evolutionary population simulations
- Strategic Complexity Budget experiments

The command-line interface was designed such that the same executable can be used for baseline tournaments, noise experiments, exploitation tests and evolutionary simulations.

## Strategies
Implemented strategies include:
- **ALLC**: Always Cooperate
- **ALLD**: Always Defect
- **TFT**: Tit-for-Tat
- **GRIM**: Grim Trigger
- **RND(p): Random strategy with configurable cooperation probability
- **PAVLOV**: Win-stay, Lose-shift
- **CONTRITE/CTFT**: Contrite Tit-for-Tat
- **PROBER**: Probing Tit-for-Tat variant

### Original Strategies
#### CAUTIOUS
CAUTIOUS begins by defecting for its first two rounds before employing the win-stay, lose-shift strategy. If it becomes trapped in repeated mutual defection, it deliberately cooperates for two rounds in an attempt to restore cooperation before returning to its normal decision logic. 

The strategy was designed to combine an initially defensive approach with the ability to recover cooperation after noisy or retaliatory responses from its opponent.

#### Firm but Fair (FBF)
This strategy begins cooperatively but keeps track of previous exploitation. When betrayed, it retaliates proportionally to the number of previous betrayals. After punishment, it attempts to re-establish cooperation, and repeated mutual cooperation gradually reduces its stored distrust.

FBF also tracks whether an opponent has previously demonstrated cooperative behaviour, allowing it to tolerate isolated defections from trusted opponents (e.g. from noise), while remaining resistant to persistent exploiters.

## Architecture
The simulator is structured around an engine that owns the high-level execution flow and validates command-line configuration before tournaments are run.

Strategies derive from a common polymorphic base class and provide their own decision behaviour through an overridden strategy interface. This allows tournament code to operate on strategies without depending on their concrete type.

The implementation makes use of:
- OOP strategy classes
- Inheritance and runtime polymorphism
- `std::unique_ptrs` for strategy ownership
- Enumerations for strategy identifiers
- Map structures for strategy-associated data
- Custom structures for match and tournament results
- CMake for building the executable and library

Each strategy is reset between repeated matches so the state from one experiment does not affect the next.

## Command-Line Interface
Arguments:
```
--rounds L
--repeats RPT
--epsilon E
--seed S
--payoffs T,R,P,S
--strategies LIST  # e.g., ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER
--format {text|json|csv}
--save FILE
--load FILE
--evolve
--population N
--generations G
--mutation MU
--scb
```
Example baseline tournament (assuming the build directions outlined in this direction are followed):
``` cmd
build\Debug\IPD_main.exe --rounds 120 --repeats 12 --seed 7 --strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format text
```

Example noisy tournament:
``` cmd
build\Debug\IPD_main.exe --rounds 120 --repeats 12 --epsilon 0.05 --seed 14 --strategies TFT,GRIM,PAVLOV,CONTRITE --format csv
```

Example evolutionary tournament:
``` cmd
build\Debug\IPD_main.exe --rounds 120 --repeats 12 --seed 21 --strategies FBF,ALLC,TFT,GRIM,RND0.75 --format json --evolve --population 200 --generations 50
```

## Experiments
This project was used to investigate several aspects of Iterated Prisoner's Dilemma behaviour:
- Baseline round-robin performance
- Reciprocity under increasing noise
- Resistance to exploitative strategies
- Evolutionary selection over multiple generations
- The effect of penalising strategy complexity

### Noise
Noise is represented by `epsilon`, the probability that an agent's decision is flipped.

Experiments run with noise enabled showed how forgiving and unforgiving strategies respond differently when errors accumulate. The report included in this repository shows GRIM degraded rapidly when small amounts of noise were introduced because a single accidental defection would trigger permanent retaliation. CTFT was least affected by noise as it is able to recover cooperative play.

### Evolutionary Tournament Simulation
The simulator supports evolutionary tournaments where strategy population shares change across generations according to their fitness. The report highlights that, in an environment where cooperative strategies are the majority, their shares stabilise, whilst defecting/exploitative strategies become extinct. Furthermore, evolutionary tournaments showcased compatible strategies.

The outcome of an evolutionary tournament heavily depend on the initial strategies chosen.

### Strategy Complexity Budget (SCB)
Payoff scores are awarded according to the results of a single round. This projects explores the outcome after subtracting a certain value from the payoff determined by the complexity of the strategy. More complex strategies therefore have to achieve sufficiently high payoffs to justify their additional costs.

The project compares evolutionary tournaments with and without this penalty to investigate the trade-off between strategic complexity and simplicity. The coursework specifically required exploring whether simpler but robust strategies survive when complexity carries a cost.

## Building and Running
Requirements:
- C++20 compatible compiler
- CMake (Minimum: 4.0)

From the repository root (cmd):
```cmd
cmake -S . -B build
cmake --build build
```

After running the above commands, the executable should be stored in `build\Debug\IPD_main.exe`

To run the simulator (with appropriate commands):
```cmd
build\Debug\IPD_main.exe
```

## Results
Results are stored in the `results\` directory, and can be output as a text file, CSV file or JSON file. Tournament summaries include average strategy performance and 95% confidence intervals across repeated runs.

## Report
The full coursework report, including experiment design, graphs and analysis, is available in [Report PDF](https://github.com/Keevunn/IteratedPrisonersDilemma/blob/master/CSC8501_Report.pdf)
