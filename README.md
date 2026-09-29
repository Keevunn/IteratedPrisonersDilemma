# Iterated Prisoner's Dilemma

A configurable C++ simulation framework for running and analysing Iterated Prisoner's Dilemma tournaments.

The project was developed to investigate the behaviour of different strategies under various conditions and repeated iterations. It supports configurable round-robin tournaments as well as evolutionary population simulations.

## Overview

The simulator runs repeated prisoner's dilemma matches between configurable strategies and aggregates their performance across tournaments.

The engine supports:
- Double round-robin tournaments
- Configurable rounds and repeats
- Implementation noise
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
- **PAVLOV**: Win-stay, Lose-shift
- **CONTRITE/CTFT**: Contrite Tit-for-Tat
- **PROBER**: Probing Tit-for-Tat variant

### Original Strategies
#### CAUTIOUS
CAUTIOUS begins by defecting for its first two rounds before employing the win-stay, lose-shift strategy. If it becomes trapped in repeated mutual defection, it deliberately cooperates for two rounds in an attempt to restore cooperation before returning to its normal decision logic. 

The strategy was designed to combine an initially defensive approach with the ability to recover cooperation after noisy or retaliatory responses from its opponent.

#### Firm but Fair (FBF)
This strategy begins cooperatively but keeps track of previous exploitation. When betrayed, it retaliates proportionally to the number of previous betrayals. After punishment, it attempts to re-establish cooperation, and repeated mutual cooperation gradually reduces its stored distrust tracker.

FBF also tracks whether an opponent has previously demonstrated cooperative behaviour, allowing it to tolerate isolated defections from trusted opponents (e.g. from noise), while remaining resistant to persistent exploiters.

## Architecture
The simulator is structured around an engine that owns the high-level execution flow and validates command-line configuration before tournaments are run.

Strategies derive from a common polymorphic base class and provide their own decision behaviour through an overridden strategy interface. This allows tournament code to operate on strategies without depending on their concrete type.

The implementation makes use of:
- OOP strategy classes
- Inheritance and runtime polymorphism
- Data ownership through C++ smart pointers
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
build\Debug\IPD_main.exe --rounds 120 --repeats 12 --seed 21 --strategies FBF,ALLC,TFT,GRIM,CAUTIOUS --format json --evolve --population 200 --generations 50
```

## Experiments
### Noise
### Evolutionary Simulation

## Building
## Results
## Report
The full coursework report, including experiment design, graphs and analysis, is available in [Report PDF](https://github.com/Keevunn/IteratedPrisonersDilemma/blob/master/CSC8501_Report.pdf)
