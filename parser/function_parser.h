#ifndef FUNCTION_PARSER_H
#define FUNCTION_PARSER_H

class ParsedFunction {
  public:
        virtual ~ParsedFunction() = default;

        virtual double operator()(double x) const = 0;
};

#endif //FUNCTION_PARSER_H
