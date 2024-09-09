import os
import re
import json
import subprocess
import argparse
from typing import Dict, Tuple

import anthropic

client_anthropic = anthropic.Anthropic(
    # defaults to os.environ.get("ANTHROPIC_API_KEY")
    api_key="my_api_key",
)


def call_anthropic(content):
    response = client_anthropic.messages.create(
        model="claude-3-5-sonnet-20240620",
        messages=[
            {
                "role": "user",
                "content": content,
            }
        ],
        temperature=0,
        max_tokens=8192,
    )
    return response.content[0].text


def extract_solution_code(input_string: str) -> str:
    """
    Extracts the text between <solution_cpp> and </solution_cpp> tags from the given string.
    Returns the extracted solution code between the tags, or an empty string if no match is found.
    """
    pattern = r"<solution_cpp>(.*?)</solution_cpp>"
    match = re.search(pattern, input_string, re.DOTALL)
    return match.group(1).strip() if match else ""


def read_file(file_path: str) -> str:
    """
    Reads the contents of a file and returns it as a string.
    If the file is not found, returns a message indicating that.
    """
    try:
        with open(file_path, "r") as file:
            return file.read()
    except FileNotFoundError:
        return f"File {file_path} not found."


def create_problem_context(problem_name: str) -> str:
    """
    Generates a problem-solving context by reading the necessary files,
    such as the problem statement, input/output examples, and grader.
    Returns the context as a formatted string.
    """
    problem_statement = read_file(f"{problem_name}/en.md")
    solution_header = read_file(f"{problem_name}/{problem_name}.h")
    grader_code = read_file(f"{problem_name}/grader.cpp")
    example_input = read_file(f"{problem_name}/01.in")
    example_output = read_file(f"{problem_name}/01.out")

    context_template = """
<problem_statement>
{{statement}}
</problem_statement>

<example_in>
{{input}}
</example_in>

<example_out>
{{output}}
</example_out>

<grader_cpp>
{{grader}}
</grader_cpp>

<solution_h>
{{header}}
</solution_h>

Solve the problem with your code in single "<solution_cpp>" tag, 
your implementation will be used by "grader" to transform input to output.
"""

    context = (
        context_template.replace("{{statement}}", problem_statement)
        .replace("{{input}}", example_input)
        .replace("{{output}}", example_output)
        .replace("{{grader}}", grader_code)
        .replace("{{header}}", solution_header)
    )

    return context


def write_file(file_path: str, content: str) -> None:
    """
    Writes the given content to a specified file.
    """
    with open(file_path, "w") as file:
        file.write(content)


def setup_compilation_scripts(problem_name: str, solution_method: str) -> None:
    """
    Sets up compilation and execution scripts for the given problem and solution method.
    The scripts are written to the appropriate files in the problem's directory.
    """
    os.makedirs(f"{problem_name}/{solution_method}", exist_ok=True)
    solution_code = read_file(f"{problem_name}/{solution_method}.cpp")
    write_file(f"{problem_name}/{problem_name}.cpp", solution_code)

    compile_script_content = f"""#!/bin/bash
cd {problem_name}
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o {problem_name} grader.cpp {problem_name}.cpp
"""
    write_file(f"{problem_name}/compile_cpp.sh", compile_script_content)

    run_script_content = f"""#!/bin/bash
cd {problem_name}
ulimit -v 2097152
ulimit -s 2097152
"""
    for test_file in os.listdir(f"{problem_name}/tests/"):
        if test_file.endswith(".in"):
            test_case_name = test_file.split(".")[0]
            run_script_content += f"timeout 5s ./{problem_name} < ./tests/{test_case_name}.in > ./{solution_method}/{test_case_name}.out\n"

    write_file(f"{problem_name}/run_cpp.sh", run_script_content)


def compile_and_execute(problem_name: str, solution_method: str) -> None:
    """
    Compiles the solution for the given problem and executes the tests.
    Compilation and execution results are printed.
    """
    setup_compilation_scripts(problem_name, solution_method)

    compile_process = subprocess.run(
        ["bash", f"{problem_name}/compile_cpp.sh"], capture_output=True, text=True
    )
    print("Compilation Output:", compile_process.stdout)
    print("Compilation Error (if any):", compile_process.stderr)

    run_process = subprocess.run(
        ["bash", f"{problem_name}/run_cpp.sh"], capture_output=True, text=True
    )
    print("Execution Output:", run_process.stdout)
    print("Execution Error (if any):", run_process.stderr)


def evaluate_test_results(problem_name: str, solution_method: str) -> None:
    """
    Evaluates the test results by comparing the program output with the ground truth.
    Subtask results are scored based on test case outputs, and the total score is computed.
    """
    test_case_scores = get_test_case_scores(problem_name, solution_method)
    subtask_scores, total_score = compute_subtask_scores(problem_name, test_case_scores)
    display_test_and_subtask_scores(test_case_scores, subtask_scores, total_score)


def get_test_case_scores(problem_name: str, solution_method: str) -> Dict[str, int]:
    """
    Compares the output of each test case with the expected output and returns
    a dictionary of test case names mapped to their scores (1 for correct, 0 for incorrect).
    """
    test_case_scores = {}

    for test_file in os.listdir(f"{problem_name}/tests/"):
        if test_file.endswith(".out"):
            test_case_name = test_file.split(".")[0]
            expected_output = read_file(f"{problem_name}/tests/{test_file}")
            generated_output_file = f"{problem_name}/{solution_method}/{test_file}"

            if os.path.exists(generated_output_file):
                generated_output = read_file(generated_output_file)
                test_case_scores[test_case_name] = (
                    1 if generated_output == expected_output else 0
                )
            else:
                test_case_scores[test_case_name] = 0

    return test_case_scores


def compute_subtask_scores(
    problem_name: str, test_case_scores: Dict[str, int]
) -> Tuple[Dict[str, int], int]:
    """
    Loads subtask data and computes the subtask scores based on the minimum score of the associated test cases.
    Returns a dictionary of subtask names mapped to their scores and the total score.
    """
    subtask_data = load_subtask_data(problem_name)

    total_score = 0
    subtask_scores = {}

    for subtask_name, subtask_info in subtask_data.items():
        subtask_weight = subtask_info["score"]
        subtask_test_cases = subtask_info["testcases"]

        # Calculate the score for the subtask as the minimum score among its test cases
        subtask_score = subtask_weight * min(
            test_case_scores[test_case] for test_case in subtask_test_cases
        )

        subtask_scores[subtask_name] = subtask_score
        total_score += subtask_score

    return subtask_scores, total_score


def load_subtask_data(problem_name: str) -> Dict[str, Dict]:
    """
    Loads subtask information from JSON files in the problem directory.
    Returns a dictionary of subtask data.
    """
    subtask_data = {}

    for file_name in os.listdir(f"{problem_name}"):
        if file_name.endswith(".json"):
            with open(os.path.join(f"{problem_name}", file_name), "r") as json_file:
                subtask_info = json.load(json_file)
                subtask_data[file_name.split(".")[0]] = subtask_info

    return subtask_data


def display_test_and_subtask_scores(
    test_case_scores: Dict[str, int], subtask_scores: Dict[str, int], total_score: int
) -> None:
    """
    Displays the test case scores, subtask scores, and total score.
    """
    print("\nTest Case Scores:")
    for test_case, score in test_case_scores.items():
        print(f"  Test Case: {test_case} -> Score: {score}")

    print("\nSubtask Scores:")
    for subtask_name, score in subtask_scores.items():
        print(f"  Subtask: {subtask_name} -> Score: {score}")

    print(f"\nTotal Score: {total_score}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Evaluate solutions for competitive programming problems."
    )
    parser.add_argument(
        "--solution",
        type=str,
        required=True,
        help="Name of the solution method to evaluate",
    )

    args = parser.parse_args()
    solution_method_name = args.solution
    problems_to_solve = ["nile", "message", "tree", "hieroglyphs", "mosaic", "sphinx"]

    for problem in problems_to_solve:
        print("\n--------------------------------\n\nProblem:", problem)

        # # Model: claude-3-5-sonnet
        # problem_context = create_problem_context(problem)
        # model_solution = call_anthropic(problem_context)
        # solution_code = extract_solution_code(model_solution)
        # write_file(f"{problem}/{solution_method_name}.cpp", solution_code)

        compile_and_execute(problem, solution_method_name)
        evaluate_test_results(problem, solution_method_name)
        
# python eval.py --solution solution > solution.txt
# python eval.py --solution claude-3-5-sonnet > sonnet.txt
