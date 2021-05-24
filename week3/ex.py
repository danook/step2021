# Reads a number from |line| (starts from |index|) and
# returns a token of the number and the next index
def readNumber(line, index):
  number = 0
  while index < len(line) and line[index].isdigit():
    number = number * 10 + int(line[index])
    index += 1
  if index < len(line) and line[index] == '.':
    index += 1
    keta = 0.1
    while index < len(line) and line[index].isdigit():
      number += int(line[index]) * keta
      keta /= 10
      index += 1
  token = {'type': 'NUMBER', 'number': number}
  return token, index

# Reads an operator from line[index]
# returns a token of the operator and next index
def readOperator(line, index, brackets):
  operator_type_names = {'+': 'PLUS', '-': 'MINUS', '*': 'MULTIPLY', '/': 'DIVIDE'}
  operator_priorities = {'+': brackets * 2, '-': brackets * 2, '*': brackets * 2 + 1, '/': brackets * 2 + 1}
  operator = line[index]
  token = {'type': operator_type_names[operator], 'priority': operator_priorities[operator]}
  return token, index + 1

# Converts the line to tokens
def tokenize(line):
  tokens = []
  index = 0
  brackets = 0  # the number of brackets the token is in
  while index < len(line):
    if line[index].isdigit():
      (token, index) = readNumber(line, index)
      tokens.append(token)
    elif line[index] in ('+', '-', '*', '/'):
      (token, index) = readOperator(line, index, brackets)
      tokens.append(token)
    elif line[index] == '(':
      brackets += 1
      index += 1
    elif line[index] == ')':
      brackets -= 1
      index += 1
    else:
      print('Invalid character found: ' + line[index])
      exit(1)
    
  return tokens

# Returns maximum priority of operators in tokens
def getMaxPriority(tokens):
  max_priority = 0
  for token in tokens:
    if 'priority' in token:
      if token['priority'] > max_priority:
        max_priority = token['priority']
  return max_priority

# Calculates a continuous part of |tokens| starting from |first_index| 
# where operators' priority is |priority|
# and returns a token of the evaluated value and next index
def partialEvaluate(tokens, first_index, priority):
  answer = tokens[first_index]['number']
  index = first_index + 1

  while index < len(tokens):
    if tokens[index]['type'] == 'NUMBER':
      if tokens[index - 1]['type'] == 'PLUS':
        answer += tokens[index]['number']
      elif tokens[index - 1]['type'] == 'MINUS':
        answer -= tokens[index]['number']
      elif tokens[index - 1]['type'] == 'MULTIPLY':
        answer *= tokens[index]['number']
      elif tokens[index - 1]['type'] == 'DIVIDE':
        answer /= tokens[index]['number']
      else: # No operator before number
        print('Invalid syntax')
        exit(1)
    elif tokens[index]['priority'] != priority:
      break
    index += 1

  token = {'type': 'NUMBER', 'number': answer}
  return token, index

# Calculates |tokens| and returns the answer
def evaluate(tokens):
  max_priority = getMaxPriority(tokens)

  # Executes calculation from operators with larger priority
  for priority in reversed(range(max_priority + 1)):
    index = 0
    new_tokens = []

    while index < len(tokens):
      if index == len(tokens) - 1:  # This case has to be separated to avoid out-of-range reference
        new_tokens.append(tokens[index])
        break
      elif tokens[index]['type'] == 'NUMBER' and tokens[index + 1]['priority'] == priority:
        (new_token, index) = partialEvaluate(tokens, index, priority)
        new_tokens.append(new_token)
      else:
        new_tokens.append(tokens[index])
        index += 1

    tokens = new_tokens # Renew tokens
    
  return tokens[0]['number']


def test(line):
  tokens = tokenize(line)
  actualAnswer = evaluate(tokens)
  expectedAnswer = eval(line)
  if abs(actualAnswer - expectedAnswer) < 1e-8:
    print("PASS! (%s = %f)" % (line, expectedAnswer))
  else:
    print("FAIL! (%s should be %f but was %f)" % (line, expectedAnswer, actualAnswer))


# Add more tests to this function :)
def runTest():
  print("==== Test started! ====")
  
  # Single number
  test("3.0")
  test("1234.56")

  # + and -
  test("1+2")
  test("1.0+2.1-3")
  test("12345-54321+11111")
  test("0.15+10-123.34")
  test("1+2+3+4-5-6+7-8+9")

  # * and /
  test("3*7")
  test("4*0/12")
  test("3/100*3")
  test("10000*12345/33333")
  test("0.35/0.46/0.99")

  # Mixture of +- and */
  test("1+2*3-6/3")
  test("0.1*0.02+0.5")
  
  # Brackets
  test("(1+2+3)*(4+5*6)")
  test("((((1+2)*3+4)*5-6)/7+8)*9+0")
  test("(1+(2+3*(4+5))*6)*7/(8+9)")
  test("3+(4*5)")
  test("(4*5)+3")
  test("(4.2*5.6)+3.125")
  test("(0.4*(6/3))*(5*7)")

  print("==== Test finished! ====\n")

if __name__ == '__main__':
  runTest()

