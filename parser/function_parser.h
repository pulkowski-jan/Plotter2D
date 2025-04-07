#ifndef FUNCTION_PARSER_H
#define FUNCTION_PARSER_H
#include <string>
#include <vector>
#include "parsed_function.h"

class FunctionParser {
    public:
        ParsedFunction* parsePolishNotation(const std::string& str);

    private:
        struct Token {
            enum Type {
                NUMERIC, VARIABLE, OPERATOR, INVALID
            };

            Token(const std::string& value, Type type);

            const std::string value;
            const Type type;
        };

        static std::vector<Token> tokenize(const std::string& str);

        static Token::Type findType(const std::string& token);

        static void pushToken(std::vector<Token>& tokens, const std::string& tokenBuffer);

        static bool isOperator(const char& c);

        static bool isNumber(const std::string& token);

        static bool isVariable(const std::string& token);

        static double calc(double a, const std::string& op, double b);

        class PolishNotationFunction final : public ParsedFunction {
            public:
                struct BinaryTree {
                    Token token;
                    double value;
                    BinaryTree* left;
                    BinaryTree* right;

                    explicit BinaryTree(const Token& token);
                };

                explicit PolishNotationFunction(BinaryTree* root);

                ~PolishNotationFunction() override;

                double operator()(double x) const override;

            private:
                static void walkDelete(const BinaryTree* node);

                static double eval(const BinaryTree* node, double x);

                BinaryTree* tree;
        };

        static PolishNotationFunction::BinaryTree* parseTree(
            std::vector<Token>::const_iterator& begin, std::vector<Token>::const_iterator& end);

        class ConstFunction final : public ParsedFunction {
            public:
                explicit ConstFunction(const std::vector<Token>& tokens);

                ~ConstFunction() override;

                double operator()(double x) const override;

            private:
                double value;

                static double eval(std::vector<Token>::const_iterator& begin,
                                   std::vector<Token>::const_iterator& end);
        };
};


#endif //FUNCTION_PARSER_H
