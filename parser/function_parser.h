#ifndef FUNCTION_PARSER_H
#define FUNCTION_PARSER_H

class ParsedFunction {
  public:
    ParsedFunction() = delete;
    virtual double operator()(double x) const;
};

#endif //FUNCTION_PARSER_H
