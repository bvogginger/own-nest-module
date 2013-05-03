#ifndef STP_SCHEMMEL_CONNECTION_H
#define STP_SCHEMMEL_CONNECTION_H

/*
 *  stp_schemmel_connection.h
 *
 *  This file is part of NEST
 *
 *  Copyright (C) 2008 by
 *  The NEST Initiative
 *
 *  See the file AUTHORS for details.
 *
 *  Permission is granted to compile and modify
 *  this file for non-commercial use.
 *  See the file LICENSE for details.
 *
 */

#include "connection_het_wd.h"

/* BeginDocumentation
  Name: stp_schemmel - Short term plasticity model by Johannes Schemmel implemented in HICANN

  Description:
  Implements the short term plasticity mechanism implemented in the FACETS / BrainScaleS wafer-scale Hardware

  Transmits: SpikeEvent
  
  SeeAlso: synapsedict
*/

namespace mynest {

/**
 * Connection class for illustration purposes.
 */
class STPSchemmelConnection : public nest::ConnectionHetWD
{
 public:

  /**
   * Default Constructor.
   * Sets default values for all parameters. Needed by GenericConnectorModel.
   */
  STPSchemmelConnection();

  /**
   * Default Destructor.
   */
  ~STPSchemmelConnection() {}

  /**
   * Get all properties of this connection and put them into a dictionary.
   */
  void get_status(DictionaryDatum & d) const;
  
  /**
   * Set properties of this connection from the values given in dictionary.
   */
  void set_status(const DictionaryDatum & d, nest::ConnectorModel &cm);

  /**
   * Set properties of this connection from position p in the properties
   * array given in dictionary.
   */  
  void set_status(const DictionaryDatum & d, nest::index p, nest::ConnectorModel &cm);

  /**
   * Create new empty arrays for the properties of this connection in the given
   * dictionary. It is assumed that they are not existing before.
   */
  void initialize_property_arrays(DictionaryDatum & d) const;

  /**
   * Append properties of this connection to the given dictionary. If the
   * dictionary is empty, new arrays are created first.
   */
  void append_properties(DictionaryDatum & d) const;

  /**
   * Send an event to the receiver of this connection.
   * @param e The event to send
   * @param t_lastspike Point in time of last spike sent.
   * @param cp Common properties to all synapses (empty).
   */
  void send(nest::Event& e, nest::double_t t_lastspike, 
            const nest::CommonSynapseProperties &cp);

  //! Defining this as empty means we can handle spike events
  using Connection::check_event;  // see http://www.gotw.ca/gotw/005.htm
  void check_event(nest::SpikeEvent&) {}

 private:
  nest::double_t tau_stp_;  //!< time constant in ms
  nest::double_t U_;        //!< Usable Efficacy
  nest::double_t I_;        //!< active partition I (at last spike)
  nest::double_t lambda_;   //!< scaling factor of active partition effect
  nest::double_t beta_;     //!< bias factor for facilitation
  bool dep_;                //!< use depression mode (true = depression, false = faciliation)

};


inline
void STPSchemmelConnection::send(nest::Event& e, nest::double_t last, 
                                  const nest::CommonSynapseProperties &props)
{
	// time elapsed since last spike
	nest::double_t h = e.get_stamp().get_ms() - last;
	// update active partition
	I_ *= std::exp(-h/tau_stp_);

	nest::double_t weight_multiplier;
	// depression mode
	if (dep_)
		weight_multiplier = 1.0 - lambda_*I_;
	// facilitation mode
	else
		weight_multiplier = 1.0 + lambda_*(I_-beta_);

	// technical restriction: weight is between 0 and 2 times the static weight
	if (weight_multiplier > 2.0)
		weight_multiplier = 2.0;

	// update active partition
	I_ += U_*(1.0-I_);

	// send the spike to the target
	e.set_receiver(*target_);
	e.set_weight( weight_ * weight_multiplier );
	e.set_delay( delay_ );
	e.set_rport( rport_ );
	e();
}
 
} // namespace

#endif // stp_schemmel_connection.h
