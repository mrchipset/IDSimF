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
 test_velocityIntegrator.cpp

 Testing of pure velocity integrator for particle simulation

 ****************************/

#include "PSim_velocityIntegrator.hpp"
#include "Core_vector.hpp"
#include "BTree_particle.hpp"
#include "catch.hpp"

TEST_CASE( "Test velocity integrator", "[ParticleSimulation][VelocityIntegrator][trajectory integration]") {

    SECTION( "Velocity integrator should be able to integrate correctly particles") {

        //Test with verlet integration:
        double nParticles = 10;
        double dt = 1e-4;
        double timeSteps = 50;

        double ionVelocity = 10.0;

        auto velocityFct = [ionVelocity](BTree::Particle *particle, int particleIndex, double time, int timestep){
            Core::Vector result(ionVelocity, 0, ionVelocity*0.1);
            return (result);
        };

        auto timestepWriteFct = [](std::vector<BTree::Particle*>& particles, double time, int timestep,
                                   bool lastTimestep){};

        auto otherActionsFct = [] (
                Core::Vector& newPartPos,BTree::Particle* particle,
                int particleIndex, double time,int timestep){
        };

        std::vector<BTree::uniquePartPtr>particles;
        std::vector<BTree::Particle*>particlesPtrs;

        double yPos = 0;
        for (int i=0; i<nParticles; i++){
            BTree::uniquePartPtr particle = std::make_unique<BTree::Particle>(
                    Core::Vector(0.0,yPos,0.0),
                    Core::Vector(0.0,0.0,0.0),
                    1.0,
                    100.0);
            particlesPtrs.push_back(particle.get());
            particles.push_back(std::move(particle));
            yPos = yPos+0.01;
        }

        ParticleSimulation::VelocityIntegrator velocityIntegrator( particlesPtrs, velocityFct, timestepWriteFct,otherActionsFct);

        velocityIntegrator.runSingleStep(dt);
        velocityIntegrator.run(timeSteps,dt);

        for (int i=0; i<nParticles; i++){
            Core::Vector ionPos = particles[i]-> getLocation();
            REQUIRE(Approx(ionPos.x()).epsilon(1e-6) == 510*dt);
        }
    }
}