/*
 *  stp_schemmel_connection.cpp
 *
 *  This file is part of NEST
 *
 *  Copyright (C) 2004 by
 *  The NEST Initiative
 *
 *  See the file AUTHORS for details.
 *
 *  Permission is granted to compile and modify
 *  this file for non-commercial use.
 *  See the file LICENSE for details.
 *
 */

#include "stp_schemmel_connection.h"
#include "network.h"
#include "connector_model.h"

namespace mynest
{

  STPSchemmelConnection::STPSchemmelConnection() :
    ConnectionHetWD(),
    tau_stp_(100.),
    U_(0.5),
    I_(0.0),
    lambda_(1.0),
    beta_(0.5),
    dep_(true)
  { }

  void STPSchemmelConnection::get_status(DictionaryDatum & d) const
  {
    ConnectionHetWD::get_status(d);

    def<nest::double_t>(d, "tau_stp", tau_stp_);
    def<nest::double_t>(d, "U", U_);
    def<nest::double_t>(d, "I", I_);
    def<nest::double_t>(d, "lambda", lambda_);
    def<nest::double_t>(d, "beta", beta_);
    def<bool>(d, "dep", dep_);
  }
  
  void STPSchemmelConnection::set_status(const DictionaryDatum & d, nest::ConnectorModel &cm)
  {
    ConnectionHetWD::set_status(d, cm);

    updateValue<nest::double_t>(d, "tau_stp", tau_stp_);
    updateValue<nest::double_t>(d, "U", U_);
    updateValue<nest::double_t>(d, "I", I_);
    updateValue<nest::double_t>(d, "lambda", lambda_);
    updateValue<nest::double_t>(d, "beta", beta_);
    updateValue<bool>(d, "dep", dep_);
  }

  /**
   * Set properties of this connection from position p in the properties
   * array given in dictionary.
   */  
  void STPSchemmelConnection::set_status(const DictionaryDatum & d, nest::index p, nest::ConnectorModel &cm)
  {
    ConnectionHetWD::set_status(d, p, cm);

	nest::set_property<nest::double_t>(d, "tau_stps", p, tau_stp_);
	nest::set_property<nest::double_t>(d, "Us", p, U_);
	nest::set_property<nest::double_t>(d, "Is", p, I_);
	nest::set_property<nest::double_t>(d, "lambdas", p, lambda_);
    nest::set_property<nest::double_t>(d, "beta", p, beta_);
    nest::set_property<bool>          (d, "dep", p, dep_);
  }

  void STPSchemmelConnection::initialize_property_arrays(DictionaryDatum & d) const
  {
    ConnectionHetWD::initialize_property_arrays(d);

    initialize_property_array(d, "tau_stps");
    initialize_property_array(d, "Us");
    initialize_property_array(d, "Is");
    initialize_property_array(d, "lambdas");
    initialize_property_array(d, "betas");
    initialize_property_array(d, "deps");
  }

  /**
   * Append properties of this connection to the given dictionary. If the
   * dictionary is empty, new arrays are created first.
   */
  void STPSchemmelConnection::append_properties(DictionaryDatum & d) const
  {
    ConnectionHetWD::append_properties(d);

    append_property<nest::double_t>(d, "tau_stps", tau_stp_);
    append_property<nest::double_t>(d, "Us", U_);
    append_property<nest::double_t>(d, "Is", I_);
    append_property<nest::double_t>(d, "lambdas", lambda_);
    append_property<nest::double_t>(d, "beta", beta_);
    append_property<bool>          (d, "dep", dep_);
  }

} // of namespace mynest
