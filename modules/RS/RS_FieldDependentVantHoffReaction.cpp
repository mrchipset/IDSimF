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
 ****************************/

#include "RS_FieldDependentVantHoffReaction.hpp"

/**
 * Default constructor: Constructs and initialized the reaction
 *
 * @param educts Map of educt species
 * @param products Map of product species
 * @param H_R the forward reaction enthalpy of this reaction
 * @param K_s the equilibrium constant between forward and backward reaction rate
 * @param kBackward the backward reaction rate (reaction rate of the background reaction)
 * @param electricMobility the electrical mobility (in SI units, pressure is in Pa) at
 * standard conditions (101325 Pa and 271.15 K)
 * @param collisionGasMass the molecular mass of the background gas particles (in AMU)
 * @param label a texutal label to identify the reaction
 */
RS::FieldDependentVantHoffReaction::FieldDependentVantHoffReaction(
        std::map<Substance*, int> educts,
        std::map<Substance*, int> products,
        double H_R,
        double K_s,
        double kBackward,
        double electricMobility,
        double collisionGasMass,
        std::string label):
AbstractReaction(educts, products, false, "vanthoff_field", label),
H_R_(H_R),
K_s_(K_s),
kBackward_(kBackward),
mobility_(electricMobility),
collisionGasMass_amu_(collisionGasMass),
collisionGasMass_kg_ (collisionGasMass * RS::kgPerAmu)
{}


RS::ReactionEvent RS::FieldDependentVantHoffReaction::attemptReaction(
        RS::ReactionConditions conditions, ReactiveParticle *particle, double dt) const{
    double KT = mobility_ * P0_pa_ / conditions.pressure * conditions.temperature / T0_K_;
    double ionTemperature = conditions.temperature +
                            (collisionGasMass_kg_ * pow(KT * conditions.electricField, 2.0)) /
                            (3.0 * RS::kBoltzmann);

    double k_forward =
            1.0 /
            (std::exp(H_R_ / RGas * (1.0 / ionTemperature - 1.0 / T0_K_)) * K_s_)
            * kBackward_;

    double reactionProbability = k_forward * this->staticReactionConcentration() * dt;
    bool reactionHappened = generateRandomDecision(reactionProbability);

    return ReactionEvent{reactionHappened, reactionProbability};
}

/*
 * This is a purely stochastic reaction, thus the collision based probability is always zero
 * and this method should not be called
 */
RS::ReactionEvent RS::FieldDependentVantHoffReaction::attemptReaction(
        CollisionConditions conditions, ReactiveParticle* particle) const{
    throw("Collision based reaction probability requested for purely stochastic reaction FieldDependentVantHoffReaction");
}