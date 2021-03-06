/***************************
 Ion Dynamics Simulation Framework (IDSimF)

 Copyright 2020 - Physical and Theoretical Chemistry /
 Institute of Pure and Applied Mass Spectrometry
 of the University of Wuppertal, Germany

 IDSimF is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 IDSimF is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with IDSimF.  If not, see <https://www.gnu.org/licenses/>.

 ------------
 RS-idealIsothermReactorSim.cpp

 Simple chemical kinetics in an isotherm ideally mixed reactor

 ****************************/
#include <iostream>
#include <cmath>
#include <ctime>
#include "json.h"
#include "parameterParsing.hpp"
#include "RS_Simulation.hpp"
#include "RS_SimulationConfiguration.hpp"
#include "RS_ConfigFileParser.hpp"
#include "RS_ConcentrationFileWriter.hpp"

int main(int argc, const char * argv[]) {

    // open configuration, parse configuration file =========================================
    if (argc <2){
        std::cout << "no configuration file given"<<std::endl;
        return(0);
    }
    std::string confFileName = argv[1];
    Json::Value confRoot = readConfigurationJson(confFileName);

    std::string rsConfigFileName = pathRelativeToConfFile(
            confFileName,
            stringConfParameter("reaction_configuration",confRoot));
    RS::ConfigFileParser parser = RS::ConfigFileParser();
    RS::Simulation sim = RS::Simulation(parser.parseFile(rsConfigFileName));
    RS::SimulationConfiguration* simConf = sim.simulationConfiguration();

    std::vector<int> nParticles = intVectorConfParameter("n_particles",confRoot);
    int nSteps = intConfParameter("sim_time_steps",confRoot);
    int concentrationWriteInterval = intConfParameter("concentrations_write_interval",confRoot);
    double dt_s = doubleConfParameter("dt_s",confRoot);
    double backgroundTemperature_K = doubleConfParameter("background_temperature_K",confRoot);

    std::string resultFilename;
    if (argc == 3){
        resultFilename = argv[2];
    }
    else {
        std::stringstream ss;
        ss << argv[1] << "_result.txt";
        resultFilename = ss.str();
    }

    RS::ConcentrationFileWriter resultFilewriter(resultFilename);
    // ======================================================================================

    // init simulation  =====================================================================

    // create and add simulation particles:
    int nParticlesTotal = 0;
    std::vector<uniqueReactivePartPtr> particles;
    for (int i=0; i<nParticles.size();i++) {
        RS::Substance *subst = simConf->getAllDiscreteSubstances().at(i);
        for (int k = 0; k < nParticles[i]; k++) {
            uniqueReactivePartPtr particle = std::make_unique<RS::ReactiveParticle>(subst);
            sim.addParticle(particle.get(), nParticlesTotal);
            particles.push_back(std::move(particle));
            nParticlesTotal++;
        }
    }
    RS::ReactionConditions reactionConditions = RS::ReactionConditions();
    reactionConditions.temperature = backgroundTemperature_K;
    reactionConditions.electricField = 0.0;
    reactionConditions.pressure = 0.0;

    resultFilewriter.initFile(simConf);
    // ======================================================================================


    // simulate   ===========================================================================
    clock_t begin = std::clock();

    for (int step=0; step<nSteps; step++) {
        if (step % concentrationWriteInterval ==0) {
            sim.printConcentrations();
            resultFilewriter.writeTimestep(sim);
        }

        for (int i = 0; i < nParticlesTotal; i++) {
            sim.react(i, reactionConditions, dt_s);
        }
        sim.advanceTimestep(dt_s);
    }

    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


    std::cout << "----------------------"<<std::endl;
    std::cout << "Reaction Events:"<<std::endl;
    sim.printReactionStatistics();
    std::cout << "----------------------"<<std::endl;
    std::cout << "total reaction events:" << sim.totalReactionEvents() << " ill events:" << sim.illEvents() << std::endl;
    std::cout << "ill fraction: " << sim.illEvents() / (double) sim.totalReactionEvents() << std::endl;

    std::cout << "elapsed seconds "<< elapsed_secs<<std::endl;


    // ======================================================================================

    return 0;
}
