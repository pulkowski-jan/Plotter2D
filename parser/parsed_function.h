//
// Created by dzikimlecz on 07.04.25.
//

#ifndef PARSED_FUNCTION_H
#define PARSED_FUNCTION_H

class ParsedFunction {
    public:
    virtual ~ParsedFunction() = default;

    virtual double operator()(double x) const = 0;
};

#endif //PARSED_FUNCTION_H
