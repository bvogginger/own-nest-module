import nest
#import nest.voltage_trace
import pylab as pl

nest.Install("mymodule")
nest.ResetKernel()

###############################
# create neurons and generators
###############################

# Stimulus
stimulus = nest.Create('poisson_generator')
nest.SetStatus(stimulus, {'rate': 100.})

# Parrot Neuron, just replicates spikes like a parrot(Papagei)
parrot_neuron = nest.Create('parrot_neuron')

# stimulated neuron
neuron = nest.Create('iaf_cond_exp') # LIF with conductance-based exponentially decaying synapses
nest.SetStatus(neuron,{'tau_syn_ex':5.})

###################################
# Connect stimulus to parrot neuron
# without tsodyks markram
###################################
nest.Connect(stimulus,parrot_neuron,params = { 'weight':10.})

###################################
# Connect parrot to neuron
# with tsodyks markram
###################################

# set synapse parameters:
syn_param = {"tau_psc" :  0.1,
            "tau_rec" :  200.,
            "tau_fac" :  0.,
            "U"       :  0.5,
            "delay"   :  0.1,
            "weight"  :  10.,
            "u"       :  0.0,
            "x"       :  1.0}

tso = nest.CopyModel('tsodyks_synapse', 'mytso', syn_param)

# set synapse parameters:
syn_param = {"tau_stp" :  100.,
            "U"       :  0.5,
            "delay"   :  0.1,
            "weight"  :  10.,
            "lambda"  : 1.0,
            "dep"     : True,
            "beta"    : 0.8,
            }

nest.CopyModel('stp_schemmel_synapse', 'mystp', syn_param)
nest.Connect(parrot_neuron,neuron,params = { 'weight':10.}, model="mystp")
#nest.Connect(parrot_neuron,neuron,params = syn_param, model='tsodyks_synapse')


multimeter = nest.Create('multimeter', params={
    'withtime':True,
    'record_from': ['V_m', 'g_ex', 'g_in'],
    'interval':0.1})

#voltmeter = nest.Create('voltmeter')
spike_detector_neuron = nest.Create('spike_detector')
spike_detector_parrot = nest.Create('spike_detector')
spike_detector_poisson = nest.Create('spike_detector')
spike_detector_poisson2 = nest.Create('spike_detector')
#nest.Connect(voltmeter,neuron)
nest.Connect(multimeter,neuron)
nest.Connect(neuron,spike_detector_neuron)
# Recording the spikes from the stimulus and the parrot neuron
nest.Connect(parrot_neuron,spike_detector_parrot)
nest.Connect(stimulus,spike_detector_poisson)
nest.Connect(stimulus,spike_detector_poisson2)

nest.Simulate(300.)

events = nest.GetStatus(multimeter)[0]['events']

parrot_s_times = nest.GetStatus(spike_detector_parrot)[0]['events']['times']
poisson_s_times = nest.GetStatus(spike_detector_poisson)[0]['events']['times']
poisson_s_times2 = nest.GetStatus(spike_detector_poisson2)[0]['events']['times']
print """
Interestingly, the spiketimes for the parrot neuron and the poisson generator are different!
this is because a each Connect(poisson_generator, XXX) a indiviudal poisson process is
started at every target
"""
print "Parrot Spikes:\n", parrot_s_times
print "Poisson Spikes Recorder 1:\n", poisson_s_times
print "Poisson Spikes Recorder 2:\n", poisson_s_times2

#nest.voltage_trace.from_device(voltmeter)
#pl.show()

t = events['times']

pl.clf()

pl.subplot(211)
pl.plot(t, events['V_m'])
pl.axis([0, 300, -75, -53])
pl.ylabel('Membrane potential [mV]')

pl.subplot(212)
pl.plot(t, events['g_ex'], t, events['g_in'])
pl.axis([0, 300, 0, 45])
pl.xlabel('Time [ms]')
pl.ylabel('Synaptic conductance [nS]')
pl.legend(('g_exc', 'g_inh'))

pl.show()

