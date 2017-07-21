import cProfile
import sys
import pstats
import tspanner

# for the set_method, just put the name of the method you want to call
# like 'compute_wspd'

class Profiling:
    def __init__(self):
        self.prof = cProfile.Profile()
    def set_args(self, coef, filename):
        self.coef = coef
        self.filename = filename
    def set_method(self, name):
        self.method = name
    def run(self):
        method = getattr(tspanner, self.method)
        self.prof.enable()
        #ret = self.prof.runcall(method, self.coef, self.filename)
        ret = method(self.coef, self.filename)
        self.prof.disable()
        return ret
    def print_stats(self):
        ps = pstats.Stats(self.prof, stream=sys.stdout)
        ps.print_stats()
        self.prof.clear()

