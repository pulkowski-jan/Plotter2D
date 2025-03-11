//
// Created by dzikimlecz on 11.03.25.
//

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "../model/plot_model.h"

class Visualizer {
  PlotData* plotData;
  public:
    explicit Visualizer(PlotData*);

    void render() const;
};

#endif //VISUALIZATION_H
