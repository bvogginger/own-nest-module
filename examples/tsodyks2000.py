import nest
#import nest.voltage_trace
import pylab as pl

nest.ResetKernel()

neuron = nest.Create('iaf_cond_exp')
nest.SetStatus(neuron,{'tau_syn_ex':5.})

stimulus = nest.Create('poisson_generator')

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

nest.Connect(stimulus,neuron,params = { 'weight':10.}, model="mytso")

nest.SetStatus(stimulus, {'spike_times': pl.arange(10.,200.,10.)})

multimeter = nest.Create('multimeter', params={
    'withtime':True,
    'record_from': ['V_m', 'g_ex', 'g_in'],
    'interval':0.1})

#voltmeter = nest.Create('voltmeter')
spike_detector = nest.Create('spike_detector')
#nest.Connect(voltmeter,neuron)
nest.Connect(multimeter,neuron)
nest.Connect(neuron,spike_detector)

nest.Simulate(300.)

events = nest.GetStatus(multimeter)[0]['events']

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

