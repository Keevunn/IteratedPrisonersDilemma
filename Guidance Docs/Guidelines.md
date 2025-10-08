# CSC8501 Coursework: Iterated Prisoner’s Dilemma — Strategy Design, Simulation & Analysis (2025)
## Due 16th October 10 am

### **Assessment Brief**

You are required to implement a fully functional Iterated Prisoner’s Dilemma (IPD) simulator in C++. This simulator 
will support the execution of tournaments between different strategies, produce results in multiple formats, and 
allow experimentation with noise, payoffs, and evolutionary dynamics. The assessment consists of building the 
simulation engine, implementing the required set of strategies plus two original ones, and completing a set of 
experiments to demonstrate and analyse the behaviour of your simulator and strategy (Q1–Q5) .

### **Tasks**
- Implement a match engine that runs repeated rounds of IPD between two strategies.
- Support tournament play: double round-robin, multiple repeats, fresh state each match.
- Provide a unified CLI with options to control rounds, repeats, noise, payoffs, strategies, output format, 
saving/loading configs, and running evolutionary experiments.
- Implement required strategies: ALLC, ALLD, TFT, GRIM, PAVLOV, RND(p), CONTRITE TFT, PROBER, plus two original 
strategies of your own design.
- Carry out Q1–Q5.

### **Command Line Interface**
```
--rounds L --repeats RPT --epsilon E --seed S
--payoffs T,R,P,S
--strategies LIST              # e.g., ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER
--format {text|json|csv}
--save FILE | --load FILE
--evolve 1 --population N --generations G --mutation MU
```

### **Output Requirements**
- **Text:** Rank-ordered leaderboard with mean and stdev across repeats; concise logs when appropriate.
- **CSV/JSON:** Machine-readable results for further analysis.
- **Confidence Intervals:** Report 95% CIs for means using the formula 
$\text{mean} ± 1.96 × (\text{stdev} / \sqrt {RPT})$. This illustrates the uncertainty in estimates and the 
robustness of the findings.

### **Guide to Payoffs in the Iterated Prisoner’s Dilemma**
In the Iterated Prisoner’s Dilemma (IPD), every round between two players produces a payoff for each side. 
These payoffs depend on whether each player chooses to Cooperate (C) or Defect (D). The simulator uses a 
four-number tuple to define the scores:
- **T (Temptation):** You defect while the opponent cooperates (DC).
- **R (Reward):** You both cooperate (CC). 
- **P (Punishment):** You both defect (DD). 
- **S (Sucker):** You cooperate while the opponent defects (CD).

By default, many texts use `5,3,1,0` or `3,2,1,0`. In this coursework, you can set any real numbers 
`--payoffs T,R,P,S` as long as they satisfy the rules of the Prisoner’s Dilemma.

The simulator applies these payoffs round by round, accumulating totals over the number of rounds you set 
with `--rounds`. If you run multiple repeats using `--repeats`, the game resets each time and the program 
records the mean, standard deviation, and confidence intervals. The same payoff tuple is applied to every match 
in a tournament unless you explicitly load a different configuration.

To remain a proper Prisoner’s Dilemma, the payoffs must follow two key inequalities. First, the ordering must hold:
$$$
T > R > P > S
$$$
so that the temptation to defect is higher than cooperation, which is higher than mutual defection, which is higher 
than being the sucker. Second, for long-term cooperation to be meaningful,
$$$
2R > T + S
$$$
ensuring that two rounds of mutual cooperation give a better outcome than alternating exploitation. If these 
conditions are broken, the game no longer behaves like a Prisoner’s Dilemma, and your engine should either 
reject the values or issue a warning.

For example, with `--payoffs 5,3,1,0`, the outcomes are straightforward: CC gives both players 3, DD gives both 1, 
DC gives the defector 5 and the cooperator 0, and CD gives the opposite. Over multiple rounds, these values are 
summed into match totals, then averaged across repeats. With five rounds, ALLC vs ALLD gives ALLD a total of 25 and 
ALLC 0, while ALLC vs ALLC produces 15 each, and ALLD vs ALLD gives 5 each. These simple checks are a good way to 
test your simulator.

The choice of payoffs strongly influences strategy performance. Larger temptation (T) makes defection more 
appealing and punishes cooperation. Larger reward (R) makes stable cooperation more attractive, 
favouring reciprocal strategies like Tit-for-Tat or PAVLOV. If the inequality 2R > T+S is violated, alternating 
exploitation can outperform cooperation, altering evolutionary outcomes entirely. It is also worth noting that 
scaling all four numbers up or down, or adding the same constant to all of them, does not change relative rankings 
between strategies—only the differences matter.

For most experiments, you should use a documented default. The classic choice is `--payoffs 5,3,1,0`, which is 
strict and widely cited. A gentler alternative is `--payoffs 3,2,1,0`, which preserves the ordering but reduces the 
gap between cooperation and defection. Either works, provided you are consistent and clear.

In practice, you can set payoffs on the command line, include them in the metadata of CSV or JSON outputs, and 
ensure they are stored in any saved configuration files, allowing experiments to be reproduced. Your code should 
allow floating-point numbers (for example, `4.8,3.1,1.0,-0.2`) as long as the inequalities hold, since what 
defines the game is the relative structure, not whether the numbers are integers.

In summary, the `--payoffs` option defines the scoring rules that drive the dynamics of the Iterated Prisoner’s 
Dilemma. Always check the inequalities, record the values you used, and explain how they shape strategy success. 
The payoff matrix is the hidden framework of the game — it is what determines whether cooperation can survive, 
collapse, or evolve in your tournaments.

### Strategy Guide — What the Players Actually Do

Each strategy chooses C or D every round. Strategies can remember past events in the same match and use that 
memory to decide the next move. Below each required strategy is described with examples, pros/cons, and behaviour 
under noise. I suggest creating pseudocode that maps directly to your C++ interface.

#### Strategy: _ALLC — Always Cooperate_
**Idea:**\
The strategy **always chooses “Cooperate (C)”**, regardless of the opponent’s past or present actions. It is the 
simplest “generous” strategy.
---
**Behaviour:**
- Against any opponent, ALLC will always play C. 
- It never retaliates, never adapts, never defects.
---
**First few rounds (examples):**
- **vs ALLC:**\
→ CC, CC, CC, … (both cooperate forever, both get the reward payoff R each round).
- **vs ALLD (Always Defect):**\
→ CD, CD, CD, … (ALLC always cooperates while ALLD always defects. ALLC gets the sucker payoff S each round, 
while ALLD gets the temptation payoff T).
- **vs TFT (Tit for Tat):**\
→ CC, CC, CC, … (TFT cooperates first, then mirrors ALLC, so both just cooperate forever).
---
**Pros:**
- Maximises group welfare if opponents are also cooperative.
- Works well in very trusting or altruistic populations.
- Simple to understand and implement.
---
**Cons:**
- Highly exploitable — opponents who defect always win against ALLC.
- Not robust in competitive or “mixed” populations.
- Lacks memory, retaliation, or forgiveness mechanisms.
---
**Noise behaviour:**
Even in noisy settings (where mistakes are likely to occur), ALLC cannot adjust its play. It keeps cooperating, 
so errors don’t matter to its own strategy, but they do matter for its payoff.
---
👉 In short, **ALLC represents unconditional generosity**: it promotes cooperation but at the cost of leaving 
itself completely open to exploitation.

#### Strategy: _ALLD — Always Defect_
**Idea:**\
The strategy **always chooses “Defect (D)”**, regardless of the opponent’s past or present actions. It is the 
most “selfish” strategy.
---
**Behaviour:**
- Against any opponent, ALLD will always play D.
- It never cooperates, never changes, never forgives.
---
**First few rounds (examples):**
- **vs ALLC:**\
→ DC, DC, DC, … (ALLD always defects while ALLC always cooperates. ALLD gets the temptation payoff T each round, 
while ALLC gets the sucker payoff S).
- **vs TFT (Tit for Tat):**\
→ Round 1: ALLD plays D, TFT plays C → DC (ALLD gets T).\
→ Round 2 onward: TFT mirrors the previous move (D), so both defect forever → DD, DD, … (both get the punishment 
payoff P).
- **vs ALLD:**\
→ DD, DD, … (both defect every round, both get P each time).
---
**Pros:**
- Never exploited — avoids the sucker’s payoff (S).
- Simple, predictable.
- Can dominate if the population is filled with cooperative strategies like ALLC.
---
**Cons:**
- Misses out on mutual cooperation (R) — never achieves long-term high payoffs. 
- Leads to deadlock when facing retaliatory or adaptive opponents (TFT, Grim Trigger, etc.). 
- Poor in environments where trust and reciprocity are rewarded.
---
**Noise behaviour:**\
Occasional mis-implementation (flipping to C) doesn’t alter its course: ALLD returns immediately to D, so noise 
provides no lasting change.

👉 In short, **ALLD represents unconditional selfishness**: it maximises short-term individual gain against 
cooperators, but prevents any chance of building stable cooperation.

#### Strategy: _TFT — Tit-for-Tat_
**Idea:**\
Start with Cooperate (C), then on each subsequent round copy the opponent’s previous move. It’s a “nice but 
retaliatory” strategy.
---
**Behaviour:**
- **Round 1:** Always play C. 
- **Round 2+:** Play whatever the opponent played in the previous round. 
- Forgives quickly: if the opponent returns to C, TFT will also return to C on the next turn.
---
**First few rounds (examples):**
- **vs ALLC:**\
→ CC, CC, CC, … (both always cooperate, both get reward payoff R).
- **vs ALLD:**\
→ Round 1: TFT plays C, ALLD plays D → CD (TFT gets S, ALLD gets T).\
→ Round 2 onward: TFT mirrors ALLD (D), so both defect forever → DD, DD, … (both get punishment payoff P).
- **vs TFT:**\
→ Round 1: CC (both start with C).\
→ Round 2 onward: both mirror each other’s last C → CC, CC, … (stable mutual cooperation).
---
**Pros:**
- Promotes cooperation when opponents are also cooperative. 
- Retaliatory — protects against long-term exploitation. 
- Forgiving — once the opponent returns to cooperation, TFT follows suit. 
- Very effective in evolutionary tournaments (famously successful in Axelrod’s competitions).
---
**Cons:**
- Vulnerable to noise: accidental defections can cause a chain of retaliation (“echo effect”), leading to 
unnecessary cycles of DD. 
- Never exploits overly generous strategies like ALLC (it just cooperates back, instead of taking advantage). 
- Can get stuck in deadlock if the opponent is also reactive and noise occurs.
---
**Noise behaviour:**
- If a C accidentally flips to D, TFT retaliates with D next round, the opponent retaliates again, etc. → long runs 
of DD can emerge unless a forgiving strategy intervenes. 
- Still, TFT’s “first move = C” and one-step memory makes it more forgiving than “grim” strategies.
---
👉 In short, TFT represents reciprocal cooperation: friendly at first, fair in response, but prone to breakdown 
under noise.

#### Strategy: _GRIM — Grim Trigger_
**Idea:**\
Start with Cooperate (C) and keep cooperating until the opponent defects once. After a single defection, 
switch to Defect (D) forever. No forgiveness.
---
**Behaviour:**
- Round 1: Play C. 
- If opponent always plays C: GRIM keeps playing C forever (stable cooperation). 
- If opponent defects once: GRIM switches to D permanently.
---
**First few rounds (examples):**
- **vs ALLC:**\
→ CC, CC, CC, … (both cooperate forever, payoff R every round).
- **vs ALLD:**\
→ Round 1: GRIM plays C, ALLD plays D → CD (GRIM gets S, ALLD gets T).\
→ Round 2 onward: GRIM switches to D forever → DD, DD, … (both get P).
- **vs TFT:**\
→ Both start with C.\
→ As long as TFT cooperates, GRIM cooperates.\
→ If either defects once, TFT retaliates, GRIM punishes forever, leading to endless DD.
---
**Pros:**
- Strong deterrent: the threat of permanent punishment can enforce cooperation if the opponent values long-term 
payoffs. 
- Very effective in noise-free, stable environments. 
- Simple to define and implement.
---
**Cons:**
- Unforgiving: one defection (even accidental) destroys all future cooperation.
- Vulnerable to noise: a single error leads to permanent DD cycles. 
- Can be too harsh compared to more forgiving strategies like TFT or Generous TFT.
---
**Noise behaviour:**
- If cooperation accidentally flips to defection, GRIM interprets it as betrayal.
- Result: permanent switch to D, locking both players into mutual punishment (DD forever).
- Thus, GRIM is highly unstable in noisy settings.
---
👉 In short, **GRIM represents “harsh justice”**: cooperation is maintained by the threat of eternal punishment, but
it collapses easily in imperfect environments.

Strategy: PAVLOV — Win-Stay, Lose-Shift
Idea:
Repeat the last move if it gave a “good” payoff (Win = stay).
Switch the last move if it gave a “bad” payoff (Lose = shift).
It adapts quickly, seeking payoffs better than mutual defection.

Behaviour:

Round 1: Often starts with C (convention, though not strictly defined).

Rule:

If the last outcome was CC (R) or DD (P) → stay with same choice.

If the last outcome was CD (S) or DC (T) → switch choice next time.

First few rounds (examples):

vs ALLC:
→ Starts C → CC (R, good) → keeps C → CC forever. Stable mutual cooperation.

vs ALLD:
→ Round 1: C vs D → CD (S, bad) → switches to D.
→ Round 2: D vs D → DD (P, acceptable) → stays with D forever. Stable mutual defection.

vs TFT:
→ Both start C → CC (R, good) → stay at C → CC forever. Stable cooperation.

vs GRIM:
→ Both start C → CC → stable C… unless one misstep occurs. If defection happens, PAVLOV can eventually shift back to C (more forgiving than GRIM).

Pros:

More forgiving than TFT or GRIM: can recover cooperation after accidental defections.

Exploits ALLC less harshly than ALLD, but still avoids endless exploitation.

Good balance between retaliation and forgiveness.

Cons:

Can be unstable against certain reactive strategies — may bounce between C and D if misaligned.

Vulnerable to systematic exploitation by clever alternating strategies.

More complex rule set than TFT (harder to intuit).

Noise behaviour:

If an error causes a single mistaken move (e.g. defect instead of cooperate), PAVLOV may temporarily switch, but its “win-stay/lose-shift” logic lets it return to cooperation relatively quickly.

Much more robust to noise than GRIM or TFT.

👉 In short, PAVLOV represents adaptive reciprocity: it cooperates when cooperation works, defects when it doesn’t, and can repair relationships after mistakes.

Strategy: RND(p) — Random
Idea:
On each round, play Cooperate (C) with probability p and Defect (D) with probability (1–p), regardless of history.
It is memoryless and does not adapt to the opponent.

Behaviour:

No state carried forward — every decision is fresh.

Purely probabilistic:

p = 1 → ALLC

p = 0 → ALLD

0 < p < 1 → stochastic mix of C and D

First few rounds (examples):

vs ALLC:
→ Expected outcome: ALLC always plays C; RND cooperates with prob. p (yielding R), defects with prob. 1–p (yielding T).
→ On average: RND earns p·R + (1–p)·T per round.

vs ALLD:
→ ALLD always plays D; RND cooperates with prob. p (yielding S), defects with prob. 1–p (yielding P).
→ On average: RND earns p·S + (1–p)·P.

vs TFT:
→ Random, so outcomes fluctuate: TFT mirrors previous move, which makes the interaction unpredictable — sometimes cooperation runs, sometimes defect cycles.

Pros:

Unpredictable — can be hard to exploit by deterministic opponents.

Models “irrational” or “noisy” human behaviour.

Useful as a benchmark or control in simulations.

Cons:

No strategy: does not adapt, build cooperation, or retaliate.

Performs poorly against structured opponents (e.g., TFT, PAVLOV) in long-term averages.

Highly inconsistent — payoffs vary a lot depending on p.

Noise behaviour:

Noise is redundant: the strategy already introduces randomness by design.

Still, if implementation errors shift p away from its intended value, results may differ from expectation.

👉 In short, RND(p) represents stochastic play: it captures unpredictability, but lacks the ability to form stable cooperation or deterrence.

Strategy: CTFT — Contrite Tit-for-Tat
Idea:
A refinement of Tit-for-Tat designed for noisy environments.
It still cooperates first, and normally copies the opponent’s previous move, but it recognises when its own unintended defection caused retaliation and seeks to “apologise” by returning to cooperation.

Behaviour:

Round 1: Play C.

Rule: Copy opponent’s last move, unless you defected by mistake in the previous round.

If you accidentally defected and the opponent retaliates with D, you “accept blame” and still cooperate, signalling contrition.

Prevents endless retaliation loops by correcting for one’s own errors.

First few rounds (examples):

vs ALLC:
→ CC, CC, CC … (cooperation maintained).
→ If noise causes CTFT to defect once: outcome = DC.
→ Next round, ALLC plays C, CTFT recognises its own error → cooperates → CC restored.

vs ALLD:
→ Round 1: CTFT plays C, ALLD plays D → CD (CTFT gets S, ALLD gets T).
→ Round 2: CTFT mirrors D → DD.
→ Remains at DD (stable punishment, no forgiveness to permanent ALLD).

vs TFT:
→ Starts CC, both mirror each other.
→ If noise causes CTFT to defect: DC (TFT punishes).
→ Next round, CTFT cooperates (contrition), TFT sees C and forgives → CC restored.

Pros:

Noise-resistant: avoids long retaliatory cycles caused by accidental defections.

Retains TFT’s strengths: nice (cooperates first), retaliatory (punishes exploitation), forgiving.

Promotes long-term cooperation more reliably in real-world settings with errors.

Cons:

Slightly more complex rule (needs self-awareness of its own mistakes).

Against relentless defectors (ALLD), it performs no better than TFT.

Can be “too forgiving” if the opponent exploits repeated “contrition signals.”

Noise behaviour:

CTFT is specifically designed to handle noise.

If it defects by accident, it does not spiral into endless retaliation loops: instead, it corrects by cooperating again.

Thus, CTFT achieves higher average payoffs in noisy tournaments than standard TFT.

👉 In short, CTFT represents “apologetic reciprocity”: it plays like TFT but adds self-correction, making it much more stable under noise.

Strategy: PROBER
Idea:
A “clever” variant of Tit-for-Tat that starts with a probing sequence to test the opponent’s willingness to cooperate or defect. If the opponent is exploitable, PROBER will take advantage; if not, it reverts to TFT.

Behaviour:

Round 1–4 (probe phase): Plays a fixed sequence, often C, D, C, C.

This tests the opponent’s responses.

For example, if the opponent ignores the defection on move 2 (keeps cooperating), PROBER labels them as exploitable.

After probe:

If opponent proved exploitable → PROBER defects repeatedly (to harvest T).

Otherwise → PROBER switches to Tit-for-Tat (C first, then copy).

First few rounds (examples):

vs ALLC:
→ PROBER plays C, D, C, C. ALLC keeps playing C.
→ PROBER recognises ALLC doesn’t retaliate → switches to full-time defection (DDDD…).

vs ALLD:
→ PROBER plays C, D, C, C. ALLD plays D throughout.
→ PROBER sees no benefit from exploitation, so shifts into TFT → both end up in mutual DD.

vs TFT:
→ PROBER plays C, D, C, C.
→ TFT punishes after the D, then forgives after the return to C.
→ PROBER recognises TFT retaliates → adopts TFT behaviour → CC forever.

Pros:

Can exploit unconditional cooperators like ALLC effectively.

Retains the strength of TFT against retaliatory opponents.

Flexible: adaptive depending on what the probe discovers.

Cons:

The probing phase itself incurs a cost (especially against ALLD or other defect-heavy players).

May damage trust with otherwise cooperative opponents (such as TFT), although this is recoverable.

More complex to define than straightforward TFT.

Noise behaviour:

Noise during the probe can lead to misclassification: e.g., an accidental C instead of D may make PROBER think an opponent is “safe” when they’re not.

Can cause instability if it misreads the opponent’s strategy from corrupted probe data.

👉 In short, PROBER represents “cautious opportunism”: it feels out the opponent with an opening test, exploits the weak, and cooperates with the strong.

Designing Your Own Two Strategies
Consider memory, opening, punishment, forgiveness, anti-exploitation, and clarity. Use deterministic rules and keep behaviour simple and explainable.

Example CLI Commands
# quick text baseline
./ipd --rounds 120 --repeats 12 --epsilon 0.03 --seed 7 \
--strategies ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER --format text

# noise sweep
./ipd --rounds 150 --repeats 30 --epsilon 0.00 --seed 42 --format csv
./ipd --rounds 150 --repeats 30 --epsilon 0.03 --seed 42 --format csv
./ipd --rounds 150 --repeats 30 --epsilon 0.10 --seed 42 --format csv

# evolution demo
./ipd --rounds 150 --epsilon 0.03 --seed 9 \
--strategies ALLC,ALLD,TFT,GRIM,PAVLOV,CONTRITE,PROBER \
--evolve 1 --population 200 --generations 200 --mutation 0.02 --format text
Assessment Questions (65 marks total)
Q1 (15 marks): Build the engine & CLI; run baseline tournament

Implement a Prisoner’s Dilemma tournament engine in your C++.

The engine must allow strategies to be plugged in as modules (e.g. decide(state) → C/D).

It must simulate repeated PD for a configurable number of rounds (e.g. 100) and players.

Provide a simple Command Line Interface (CLI) that allows the user to:

Choose which strategies to include (e.g. ALLC, ALLD, TFT, GRIM, PAVLOV, RND(p)).

Set the number of rounds, repetitions, and seed for randomness.

Output pairwise payoffs in a readable format (table or CSV).

Run at least one baseline round-robin tournament with ALLC, ALLD, TFT, GRIM, PAVLOV.

Present the payoff matrix and discuss which strategies do best in a noise-free environment.

Marks for: correctness (engine works), usability (CLI functions), and interpretation of results.

Q2 (10 marks): Reciprocity under noise — sweep
ε

Extend your engine to allow implementation noise: each move flips (C→D or D→C) with probability
ε.

Choose a representative set of strategies (TFT, GRIM, PAVLOV, CTFT).

Run repeated tournaments while sweeping
ε from 0 to 0.2 in steps (e.g. 0.0, 0.05, 0.1, 0.2).

Plot or tabulate the average payoff per strategy against noise level.

In your discussion, highlight:

Which strategies collapse under noise (and why).

These are resilient and maintain cooperation.

Marks for correct implementation of noise, clear presentation (graph or table), and interpretation.

Q3 (10 marks): Robustness to exploiters

Introduce exploitative strategies: PROBER and ALLD.

Run tournaments where cooperative strategies (e.g. ALLC, TFT, CTFT, PAVLOV) face exploiters.

Compare outcomes with and without noise.

Specific tasks:

Show how PROBER behaves vs ALLC and vs TFT.

Show how ALLD performs in a mixed population.

Explain why some strategies resist exploitation better.

Marks for experiments that clearly show exploitation in action, plus analysis.

Q4 (10 marks): Evolutionary selection

Implement an evolutionary tournament using replicator dynamics or simple proportional selection.

Start with a population of mixed strategies (choose at least 4).

At each generation, calculate average fitness (payoff), then update population shares accordingly.

Run for ~50 generations under both:

Noise-free play.

Noisy play (e.g.
ε=0.05).

Present graphs showing population shares over time.

Discuss which strategies dominate in each scenario and why (refer to ESS logic).

Marks for correct evolutionary mechanism, presentation, and discussion.

Q5 (20 marks): Strategic Complexity Budget (SCB) exploration

Define a strategic complexity budget (SCB): each strategy incurs a “cost” proportional to its memory or rule complexity. For example:

ALLC/ALLD = 1 (no memory).

TFT/GRIM = 2 (1-round memory).

PAVLOV = 2 (memory of last outcome).

CTFT/PROBER = 3+ (extra logic for noise/probe).

Subtract this cost from the strategy’s payoff when comparing fitness.

Investigate how SCB alters evolutionary outcomes:

Run tournaments with and without SCB applied.

Show which strategies survive when complexity is penalised.

Discuss the trade-off between sophistication and cost.

Marks for: clear definition of SCB, correct integration into payoffs, meaningful analysis, and interpretation of results (e.g. “simple but robust strategies dominate when complexity is costly”).

Programming Technique (35 marks)
Functions (5): clean decomposition (I/O, match engine, tournament, stats).
Object Orientation (5): strategy interface + classes.
Operator Overloading (5): move/result I/O, leaderboards.
Polymorphism (5): virtual dispatch.
Templates (5): payoff/score typing.
Advanced techniques  (5): - e.g., &&, lambda, threads.
Unified CLI (5): one program for all tasks.