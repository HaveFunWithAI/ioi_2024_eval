# IOI 2024 Evaluation 

This repository provides a framework for evaluating C++ solutions to the IOI 2024 problems: [IOI 2024 Tasks](https://www.ioi2024.eg/competition-tasks). 

The evaluation process scores solutions based on the performance against official test cases.

## Preparing Solutions

Each problem is represented by its own directory, and for every problem, C++ solutions should follow a specific naming convention:
- **Solution file**: `{problem}/{method_name}.cpp`

Where:
- `{problem}` is the name of the problem.
- `{method_name}` refers to the method used to solve the problem, such as an official solution or a solution created by an AI model.

### Examples:

- `nile/solution.cpp` – the official solution for the `nile` problem.
- `nile/claude-3-5-sonnet.cpp` – a model-generated solution for the `nile` problem using Claude.

## Model Solutions (for reference)

Claude-generated solutions are created using scripts such as the following (out of scope here):

```python
# Example for generating solutions with Claude 3.5 Sonnet
problem_context = create_problem_context(problem)
model_solution = call_anthropic(problem_context)
solution_code = extract_solution_code(model_solution)
write_file(f"{problem}/{method_name}.cpp", solution_code)
```

This script extracts the generated solution and saves it in the correct directory following the naming conventions described earlier.

## Evaluating Solutions

### Evaluation Script

The evaluation of solutions is handled by the `eval.py` script. This script compiles and runs the solution, then computes the score based on how well it performs on the provided test cases.

### Usage

To evaluate a specific solution, use the following command:

```bash
python eval.py --solution {method_name}
```

Replace `{method_name}` with the name of the solution to be evaluated (e.g., `solution` for the official solution or a model-generated solution like `claude-3-5-sonnet`).

#### Example Commands:
1. **Evaluating the Official Solution**:
   ```bash
   python eval.py --solution solution > solution.txt
   ```
2. **Evaluating a Model-Generated Solution** (e.g., Claude 3.5 Sonnet):
   ```bash
   python eval.py --solution claude-3-5-sonnet > sonnet.txt
   ```

### Evaluation Output

Each evaluation will generate a detailed report with scores for each test case and subtask. The scoring breakdown is as follows:

- **Test Case Score**:
  - `1` for passing test cases.
  - `0` for failing test cases.

- **Subtask Score**:  
  Calculated as the minimum test case score within the subtask, multiplied by the full score of that subtask.

- **Total Problem Score**:  
  The sum of all subtask scores, with a maximum score of 100 points per problem.

### Example Scores:

- **Official Solutions**: score all 600 points.
- **Claude-Generated Solutions**: score 22 points.

For more results of contestants, see [IOI 2024 Results](https://stats.ioinformatics.org/results/2024).
