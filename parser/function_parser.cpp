#include "function_parser.h"

#include <algorithm>
#include <cmath>
#include <regex>
#include <vector>

ParsedFunction* FunctionParser::parsePolishNotation(const std::string& str) {
    std::vector<Token> tokens = tokenize(str);
    const Token* var = nullptr;
    for (const auto& token : tokens) {
        if (token.type == Token::Type::VARIABLE) {
            if (var == nullptr) {
                var = &token;
            } else if (var->value != token.value) {
                throw std::invalid_argument("Multivariable functions are not supported");
            }
        }
    }

    if (var == nullptr) {
        return new ConstFunction(tokens);
    }
    auto begin = tokens.cbegin();
    auto end = tokens.cend();
    return new PolishNotationFunction(parseTree(begin, end));
}

std::vector<FunctionParser::Token> FunctionParser::tokenize(const std::string& str) {
    std::vector<Token> tokens;
    std::string tokenBuffer;
    for (const char& c : str) {
        if (std::isspace(c)) {
            if (!tokenBuffer.empty()) {
                pushToken(tokens, tokenBuffer);
                tokenBuffer.clear();
            }
        } else if (std::isdigit(c) || c == '.' || std::isalpha(c)) {
            tokenBuffer += c;
        } else if (isOperator(c)) {
            if (!tokenBuffer.empty()) {
                pushToken(tokens, tokenBuffer);
                tokenBuffer.clear();
            }
            tokens.emplace_back(std::string(1, c), Token::OPERATOR);
        }
    }
    if (!tokenBuffer.empty()) {
        pushToken(tokens, tokenBuffer);
    }
    return tokens;
}

double FunctionParser::PolishNotationFunction::operator()(const double x) const {
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Argument is not finite");
    }
    const BinaryTree* root = this->tree;
    return eval(root, x);
}

FunctionParser::PolishNotationFunction::PolishNotationFunction(BinaryTree* root): tree(root) { }

FunctionParser::PolishNotationFunction::~PolishNotationFunction() {
    walkDelete(tree);
}

double FunctionParser::ConstFunction::operator()(double x) const {
    return value;
}

FunctionParser::ConstFunction::ConstFunction(const std::vector<Token>& tokens) {
    auto begin = tokens.cbegin();
    auto end = tokens.cend();
    this->value = eval(begin, end);
}

FunctionParser::ConstFunction::~ConstFunction() = default;

double FunctionParser::PolishNotationFunction::eval(const BinaryTree* node, const double x) {
    if (node == nullptr) {
        throw std::invalid_argument("Invalid expression");
    }
    const auto& token = node->token;
    if (token.type == Token::Type::VARIABLE) {
        return x;
    }
    if (token.type == Token::Type::NUMERIC) {
        return node->value;
    }
    if (token.type != Token::Type::OPERATOR || node->left == nullptr || node->right == nullptr) {
        throw std::invalid_argument("Invalid expression");
    }
    return calc(eval(node->left, x), token.value, eval(node->right, x));
}

void FunctionParser::PolishNotationFunction::walkDelete(const BinaryTree* node) {
    if (node->right != nullptr) {
        walkDelete(node->right);
    }
    if (node->left != nullptr) {
        walkDelete(node->left);
    }
    delete node;
}

double FunctionParser::ConstFunction::eval(std::vector<Token>::const_iterator& begin,
                                           std::vector<Token>::const_iterator& end) {
    if (begin == end) {
        throw std::invalid_argument("Invalid expression");
    }
    if (begin->type == Token::Type::VARIABLE) {
        throw std::invalid_argument("Variable in a constant function");
    }
    if (begin->type == Token::Type::NUMERIC) {
        const double d = std::stod(begin->value);
        ++begin;
        return d;
    }
    if (begin->type != Token::Type::OPERATOR) {
        throw std::invalid_argument("Invalid token: " + begin->value);
    }
    const auto& op = begin->value;
    ++begin;
    const auto a = eval(begin, end);
    const auto b = eval(begin, end);
    return calc(a, op, b);
}

FunctionParser::PolishNotationFunction::BinaryTree* FunctionParser::parseTree(
    std::vector<Token>::const_iterator& begin, std::vector<Token>::const_iterator& end) {
    if (begin == end) {
        throw std::invalid_argument("Invalid expression");
    }
    const auto& token = *begin;
    auto* root = new PolishNotationFunction::BinaryTree(token);
    ++begin;
    if (token.type == Token::Type::NUMERIC) {
        root->value = std::stod(token.value);
    } else if (token.type == Token::Type::OPERATOR) {
        root->left = parseTree(begin, end);
        root->right = parseTree(begin, end);
    }
    return root;
}

FunctionParser::PolishNotationFunction::BinaryTree::BinaryTree(const Token& token): token(token),
    left(nullptr), right(nullptr) {
    if (token.type == Token::Type::NUMERIC) {
        value = std::stod(token.value);
    } else {
        value = NAN;
    }
}

FunctionParser::Token::Token(const std::string& value, const Type type) : value(value), type(type) {
}

FunctionParser::Token::Type FunctionParser::findType(const std::string& token) {
    if (token.size() == 1 && isOperator(token[0])) {
        return Token::Type::OPERATOR;
    }
    if (isNumber(token)) {
        return Token::Type::NUMERIC;
    }
    if (isVariable(token)) {
        return Token::Type::VARIABLE;
    }
    return Token::Type::INVALID;
}

void FunctionParser::pushToken(std::vector<Token>& tokens, const std::string& tokenBuffer) {
    const Token::Type type = findType(tokenBuffer);
    if (type != Token::INVALID) {
        tokens.emplace_back(tokenBuffer, type);
    }
}

bool FunctionParser::isNumber(const std::string& token) {
    const static std::regex numRegex(R"(\d+(\.\d*)?)");
    return std::regex_match(token, numRegex);
}

bool FunctionParser::isVariable(const std::string& token) {
    return std::isalpha(token[0]) && std::all_of(token.begin() + 1, token.end(), [](const char& c) {
        return std::isalpha(c) || std::isdigit(c);
    });
}

bool FunctionParser::isOperator(const char& c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

double FunctionParser::calc(const double a, const std::string& op, const double b) {
    if (op == "+") {
        return a + b;
    }
    if (op == "-") {
        return a - b;
    }
    if (op == "*") {
        return a * b;
    }
    if (op == "/") {
        return a / b;
    }
    if (op == "^") {
        return std::pow(a, b);
    }
    throw std::invalid_argument("Invalid token: " + op);
}
