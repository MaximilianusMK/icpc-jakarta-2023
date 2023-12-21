# ICPC Jakarta 2023 - Problem Repository

## Repository Structure

Each problem (from PA to PD for trial and A to M for contest) has their own folder. The folder name is the `slug` of the problem, which is the codename we used when we internally prepare the problems. The following table describes the slug for each problem

| Problem Code | Problem Slug | Problem Title           |
| ------------ | ------------ | ----------------------- |
| PA           | double       | Non Classical Problem   |
| PB           | flawed       | Game Show               |
| PC           | party        | Organizing Party        |
| PD           | plan         | Contingency Plan        |
| A            | abc          | Easy as ABC             |
| B            | button       | Button Pressing         |
| C            | cursed       | Cursed Game             |
| D            | rocket       | Spaceship Exploration   |
| E            | merge        | Merge Not Sort          |
| F            | max          | Maximize The Value      |
| G            | grid         | Grid Game 2             |
| H            | twin         | Twin Friends            |
| I            | unique       | Contingency Plan 2      |
| J            | bfs          | Count BFS Graph         |
| K            | xor          | Deck-Building Game      |
| L            | brackets     | Palindromic Parentheses |
| M            | triplets     | Triangle Construction   |

In the problem folder, there should be exactly the following file/folder:

- `commununicator.cpp`, if the problem type is interactive. A communicator used to interact with the solution.
- `description.pdf`. The problem statement distributed to the contestants during the contest.
- `data/`. A folder consisting of all testcases. The sample inputs given in the problem statement will be in the form of `sample/icpc-<slug>_sample_<testcase number>.in/ans`, while the hidden testcases will be in the form of `secret/icpc-<slug>_1_<testcase number>.in/ans`.
- `solution.cpp`. The official solution used to generate the outputs of the testcases
- `scorer.cpp`. A validator used to validate contestants' output. The code accepts three arguments.
  - The first parameter will be used as the filename of the input file
  - The second parameter will be used as the filename of the judge's output file
  - The third parameter will be used as the filename of the contestant's output file
  - This code will print `WA` to stdout if the contestant's output is intended to be judged `WRONG ANSWER`, while it will print `AC` to stdout if the contestants's output is intended to be judged `CORRECT`.
- `verifier.py`. A verifier used to validate the input of the testcases. The code will crash if the input violates the problem constraint, while it will exit gracefully and will not output anything if the input satisfies the problem constraint.

## Limitations

The following table describes the time limit for each problem, as broadcasted to all teams at the beginning of the contest:

| Problem Code | Problem Slug | Time Limit |
| ------------ | ------------ | ---------- |
| PA           | double       | 1 sec      |
| PB           | flawed       | 1 sec      |
| PC           | party        | 1 sec      |
| PD           | plan         | 1 sec      |
| A            | abc          | 1 sec      |
| B            | button       | 1 sec      |
| C            | cursed       | 1 sec      |
| D            | rocket       | 3 sec      |
| E            | merge        | 1 sec      |
| F            | max          | 1 sec      |
| G            | grid         | 1 sec      |
| H            | twin         | 1 sec      |
| I            | unique       | 1 sec      |
| J            | bfs          | 1 sec      |
| K            | xor          | 1 sec      |
| L            | brackets     | 1 sec      |
| M            | triplets     | 1 sec      |

The memory limit for all problems is 1024MB, as broadcasted to all teams at the beginning of the contest.

## Solutions

Note that while there is only one official published solution, we have a lot of other solutions prepared for our internal testing. These other solutions include, but not limited to:

- Solutions in several other languages
- Solutions that is intended to be incorrect (e.g. WA, TLE, etc.)
- Other correct solutions (especially on problems with more than one correct outputs for each input)

## Errata

None.

## Additional Notes

The problemset was officially deployed by the judges on [Codeforces](https://codeforces.com/contest/1906) and [TLX](https://tlx.toki.id/problems/icpc-jakarta-2023).