#include <stdint.h>

#include <bitset>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>

using namespace std;

#define INPUT_CSV_FILE "rpn-input.csv"

// test controls
#define MIN_VALUE 0
#define MAX_VALUE 100
uint8_t const table_width[] = {14, 18, 14, 18, 14, 18};

// test harness structs and params
#define VALUE_NULLPTR -999

enum command : uint16_t
{
    cmd_enter = 0,
    cmd_clear,
    cmd_pop,
    cmd_top,
    cmd_left_shift,
    cmd_right_shift,
    cmd_or,
    cmd_and,
    cmd_add,
};
vector<string> command_name = {"cmd_enter", "cmd_clear", "cmd_pop", "cmd_top", "cmd_left_shift",
                               "cmd_right_shift", "cmd_or", "cmd_and", "cmd_add"};
uint8_t const width = 16U;

/*
 * *** STUDENTS SHOULD WRITE CODE FOR THIS FUNCTION ***
 * Students should create or add any data structures needed.
 * Students should create or add any functions or classes they may need.
 */

shared_ptr<uint16_t> rpn_calc(command const cmd, uint16_t const value = 0)
{
    static stack<uint16_t> rpn_stack; // Declare the stack as static

    // Initialize result with the value provided
    uint16_t result_value = value;

    // Perform operations based on the command
    switch (cmd)
    {
    case cmd_enter:
        // Push the value onto the stack
        rpn_stack.push(result_value);

        // Return a pointer to the top of the stack
        return make_shared<uint16_t>(rpn_stack.top());
        break;
    case cmd_left_shift:
        // If the stack has fewer than 2 elements, return nullptr
        if (rpn_stack.size() < 2)
        {
            return nullptr;
        }
        else
        {
            // Pop two values from the stack
            uint16_t a = rpn_stack.top();
            rpn_stack.pop();
            uint16_t b = rpn_stack.top();
            rpn_stack.pop();

            // Perform the bitwise left shift operation
            result_value = b << a;

            // Push the result back onto the stack
            rpn_stack.push(result_value);

            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    case cmd_right_shift:
        // If the stack has fewer than 2 elements, return nullptr
        if (rpn_stack.size() < 2)
        {
            return nullptr;
        }
        else
        {
            // Pop two values from the stack
            uint16_t a = rpn_stack.top();
            rpn_stack.pop();
            uint16_t b = rpn_stack.top();
            rpn_stack.pop();

            // Perform the bitwise right shift operation
            result_value = b >> a;

            // Push the result back onto the stack
            rpn_stack.push(result_value);

            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    case cmd_pop:
        //  If the stack is empty, return nullptr
        if (rpn_stack.empty())
        {
            return nullptr;
        }
        else
        {
            // Pop one value from the stack
            rpn_stack.pop();

            // If the stack is now empty, return nullptr
            if (rpn_stack.empty())
            {
                return nullptr;
            }
            else
            {
                // Return a pointer to the top of the stack
                return make_shared<uint16_t>(rpn_stack.top());
            }
        }
        break;
    case cmd_clear:
        // Clear the stack
        while (!rpn_stack.empty())
        {
            rpn_stack.pop();
        }
        // Return nullptr
        return nullptr;
        break;
    case cmd_top:
        // If the stack is empty; return nullptr
        if (rpn_stack.empty())
        {
            return nullptr;
        }
        else
        {
            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    case cmd_or:
        // If the stack has fewer than 2 elements, return nullptr
        if (rpn_stack.size() < 2)
        {
            return nullptr;
        }
        else
        {
            // Pop two values from the stack
            uint16_t a = rpn_stack.top();
            rpn_stack.pop();
            uint16_t b = rpn_stack.top();
            rpn_stack.pop();

            // Perform the bitwise OR operation
            result_value = a | b;

            // Push the result back onto the stack
            rpn_stack.push(result_value);

            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    case cmd_and:
        // If the stack has fewer than 2 elements, return nullptr
        if (rpn_stack.size() < 2)
        {
            return nullptr;
        }
        else
        {
            // Pop two values from the stack
            uint16_t a = rpn_stack.top();
            rpn_stack.pop();
            uint16_t b = rpn_stack.top();
            rpn_stack.pop();

            // Perform the bitwise AND operation
            result_value = a & b;

            // Push the result back onto the stack
            rpn_stack.push(result_value);

            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    case cmd_add:
        // If the stack has fewer than 2 elements, return nullptr
        if (rpn_stack.size() < 2)
        {
            return nullptr;
        }
        else
        {
            // Pop two values from the stack
            uint16_t b = rpn_stack.top();
            rpn_stack.pop();
            uint16_t a = rpn_stack.top();
            rpn_stack.pop();

            result_value = 0;
            // Initialize carry
            uint16_t carry = 0;

            // Perform bitwise addition using only bitwise-operators (&, |, <<, >>)
            for (int i = 0; i < 16; ++i)
            {
                // Calculate the current bit position
                uint16_t mask = 1 << i;

                // Get the individual bits at this position
                uint16_t bit_a = (a & mask) >> i;
                uint16_t bit_b = (b & mask) >> i;

                // Calculate the sum and carry
                uint16_t sum = bit_a ^ bit_b ^ carry;
                carry = (bit_a & bit_b) | (bit_a & carry) | (bit_b & carry);

                // Set the result bit
                result_value |= (sum << i);
            }

            // Check for overflow
            if (result_value < a || result_value < b)
            {
                // Since overflow occurred, return nullptr and return stack to its previous state
                rpn_stack.push(a);
                rpn_stack.push(b);
                return nullptr;
            }

            // Push the result back onto the stack
            rpn_stack.push(result_value);

            // Return a pointer to the top of the stack
            return make_shared<uint16_t>(rpn_stack.top());
        }
        break;
    default:
        // Optional Handling for any unknown command, unnecessary do to restrictions of the program by the file provided
        break;
    }
}

/*
 * *** STUDENTS SHOULD NOT NEED TO CHANGE THE CODE BELOW. IT IS A CUSTOM TEST HARNESS. ***
 */

void header()
{
    cout << left << setw(table_width[0]) << setfill(' ') << "pass/fail";
    cout << left << setw(table_width[1]) << setfill(' ') << "command";
    cout << left << setw(table_width[2]) << setfill(' ') << "value";
    cout << left << setw(table_width[3]) << setfill(' ') << "value bits";
    cout << left << setw(table_width[4]) << setfill(' ') << "result";
    cout << left << setw(table_width[5]) << setfill(' ') << "result bits" << endl;

    cout << left << setw(table_width[0]) << setfill(' ') << "--------";
    cout << left << setw(table_width[1]) << setfill(' ') << "--------";
    cout << left << setw(table_width[2]) << setfill(' ') << "--------";
    cout << left << setw(table_width[3]) << setfill(' ') << "--------";
    cout << left << setw(table_width[4]) << setfill(' ') << "--------";
    cout << left << setw(table_width[5]) << setfill(' ') << "--------" << endl;
}

void print_row(bool const test_success, command const cmd, int16_t const value, shared_ptr<uint16_t> top_of_stack)
{
    // print results
    string const pass_fail = test_success ? "PASS" : "FAIL";
    cout << left << setw(table_width[0]) << setfill(' ') << pass_fail;
    cout << left << setw(table_width[1]) << setfill(' ') << command_name[cmd];
    if (value == VALUE_NULLPTR)
    {
        cout << left << setw(table_width[2]) << setfill(' ') << " ";
        cout << left << setw(table_width[3]) << setfill(' ') << " ";
    }
    else
    {
        cout << left << setw(table_width[2]) << setfill(' ') << value;
        cout << left << setw(table_width[3]) << setfill(' ') << bitset<width>(value);
    }

    if (top_of_stack)
    {
        cout << left << setw(table_width[4]) << setfill(' ') << *top_of_stack;
        cout << left << setw(table_width[5]) << setfill(' ') << bitset<width>(*top_of_stack) << endl;
    }
    else
    {
        cout << left << setw(table_width[4]) << setfill(' ') << " ";
        cout << left << setw(table_width[5]) << setfill(' ') << " " << endl;
    }
}

vector<string> split(string const &s, string const &delimiter)
{
    vector<string> tokens;
    size_t pos = 0;
    size_t start = 0;
    string token;
    while (pos != string::npos)
    {
        pos = s.find(",", start);
        token = s.substr(start, pos - start);
        tokens.push_back(token);
        start = pos + delimiter.length();
    }

    return tokens;
}

void init_command_map(unordered_map<string, command> &command_map)
{
    for (size_t i = 0; i < command_name.size(); i++)
    {
        string const cmd = command_name[i];
        command_map[cmd] = static_cast<command>(i);
    }
}

bool parse_csv_line(string const line, unordered_map<string, command> command_map, command &input_cmd,
                    uint16_t &input_value, int32_t &answer_value)
{
    try
    {
        vector<string> tokens = split(line, ",");

        // get command
        string cmd = tokens[0];
        // if command is  valid
        if (command_map.count(cmd) == 1)
        {
            input_cmd = command_map[cmd];
        }
        else
        {
            cout << "ERROR: Invalid command cmd=" << cmd << endl;
            return false;
        }

        // get input and value
        input_value = stoi(tokens[1].c_str());
        answer_value = stoi(tokens[2].c_str());
    }
    catch (exception const &e)
    {
        cout << "ERROR: Unable to parse input csv file, line=" << line << endl;
        cout << "ERROR: exception e=" << e.what() << endl;
        return false;
    }

    return true;
}

bool test()
{
    // open input file
    ifstream input_file(INPUT_CSV_FILE);
    if (!input_file.is_open())
    {
        cout << "ERROR: Unable to find and open the file " << INPUT_CSV_FILE << endl;
        cout << "       Make sure the path to the file is correct in your code" << endl;
        return false;
    }

    // init command map
    unordered_map<string, command> command_map;
    init_command_map(command_map);

    // print header
    header();

    bool success = true;
    bool all_test_pass = true;
    uint16_t pass = 0;
    string line;
    size_t row = 0;
    while (getline(input_file, line))
    {
        // cout << "line " << row << ":" << line << endl;
        if (row > 0)
        {
            // parse csv line
            command input_cmd;
            uint16_t input_value;
            int32_t input_answer;
            bool parse_success = parse_csv_line(line, command_map, input_cmd, input_value, input_answer);
            if (!parse_success)
            {
                return false;
            }

            // set answer value
            shared_ptr<uint16_t> answer;
            if (input_answer == VALUE_NULLPTR)
            {
                answer = nullptr;
            }
            else
            {
                answer = make_shared<uint16_t>(input_answer);
            }

            // do the command
            shared_ptr<uint16_t> result = rpn_calc(input_cmd, input_value);

            // test the results
            bool test_success = false;
            bool both_null = answer == nullptr && result == nullptr;
            bool both_same_value = answer && result && (*answer == *result);
            if (both_null || both_same_value)
            {
                pass += 1;
                test_success = true;
            }

            // update all tests
            all_test_pass = all_test_pass && test_success;

            // print row
            print_row(test_success, input_cmd, input_value, result);
        }
        row += 1;
    }

    // close input file
    input_file.close();

    // summarize results
    cout << "-------------------------------------------" << endl;
    if (all_test_pass)
    {
        cout << "SUCCESS ";
    }
    else
    {
        cout << "FAILURE ";
    }
    const size_t num_tests = row - 1;
    cout << pass << "/" << num_tests << " passed" << endl;
    cout << "-------------------------------------------" << endl;

    return success;
}

int main()
{
    if (!test())
    {
        return -1;
    }
    return 0;
}