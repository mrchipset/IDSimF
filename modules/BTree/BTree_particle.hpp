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

 BTree_particle.hpp

 Simulated charged particle

 ****************************/

#ifndef BTree_ion_hpp
#define BTree_ion_hpp

#include "Core_constants.hpp"
#include "Core_vector.hpp"
#include <unordered_map>
#include <array>
#include <memory>


namespace BTree {

    class AbstractNode;

    /**
     * Defines a simulated particle, which can be managed by a geometrical Core
     */
    class Particle {

    public:

        //Constructors:
        Particle();
        virtual ~Particle()= default;
        Particle(const Core::Vector &location, double chargeElemCharges);
        Particle(const Core::Vector &location, const Core::Vector &velocity, double chargeElemCharges, double massAMU);
        Particle(const Core::Vector &location, const Core::Vector &velocity, double chargeElemCharges, double massAMU, double timeOfBirth);
        Particle(const Core::Vector &location, const Core::Vector &velocity, double chargeElemCharges, double massAMU, double collisionDiameterM, double timeOfBirth);

        //setters and getters for the members of the particle:
        void setLocation(const Core::Vector &location);
        Core::Vector& getLocation();
        void setVelocity(const Core::Vector &velocity);
        Core::Vector& getVelocity();
        void setAcceleration(const Core::Vector &velocity);
        Core::Vector& getAcceleration();

        void setHostNode(AbstractNode* newHostNode);
        AbstractNode* getHostNode();

        void setIndex(int index);
        int getIndex();

        void setChargeElementary(double chargeElemCharges);
        double getCharge() const;
        void setActive(bool active);
        bool isActive() const;
        void setInvalid(bool invalid);
        bool isInvalid() const;

        double getAuxScalarParam(std::string key) const;
        void setAuxScalarParam(std::string key,double value);
        std::array<double, 3>& getAuxCollisionParams();

        void setMobility(double mobility);
        double getMobility() const;
        void setMeanFreePathSTP(double meanFreePathSTP);
        double getMeanFreePathSTP() const;
        void setMeanThermalVelocitySTP(double meanVelocitySTP);
        double getMeanThermalVelocitySTP() const;
        void setMassAMU(double massAMU);
        double getMass() const;
        void setDiameter(double diameter);
        double getDiameter() const;

        void setTimeOfBirth(double timeOfBirth);
        double getTimeOfBirth() const;
        void setSplatTime(double splatTime);
        double getSplatTime() const;

    private:
        //all internal variable are in SI units
        int index_; ///< an external index (mostly the index of the particle in the reaction model)
        Core::Vector location_; ///< The spatial location of the particle (m)
        Core::Vector velocity_; ///< The velocity of the particle (m/s)
        Core::Vector acceleration_; ///< The acceleration of the particle (m/s^2)
        double charge_;       ///< The charge of the particle (C)
        double mobility_;     ///< The electrical mobility of the charged particle (m^2 / (V*s))
        double mass_;         ///< The mass of the particle (kg)
        double diameter_;         ///< (collision) diameter of the particle (m^2)
        double STP_meanFreePath_; ///< mean free path at standard temperature and pressure (m)
        double STP_meanThermalVelocity_; ///< mean thermal velocity at standard temperature and pressure (m/s)

        bool active_;   ///< Flag if the particle is active in the simulation, false if the particle is already terminated
        bool invalid_;  ///< Flag if the particle has an invalid state in the simulation
        double timeOfBirth_; ///< Time when the particle was created
        double splatTime_;   ///< Time when the particle was terminated ("splatted")
        AbstractNode* hostNode_;     ///< A link to a tree node to which the particle is belonging currently

        std::unordered_map<std::string,double> auxScalarParams_; ///< an arbitrary set of additional parameters, accessible by a name
        std::array<double, 3> auxCollisionParams_ {0.0,0.0,0.0}; ///< quickly accessible parameters for collision models
    };

    typedef std::unique_ptr<BTree::Particle> uniquePartPtr;
}


#endif /* BTree_ion_hpp */