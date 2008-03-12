# encoding=UTF-8

class Container:
	pass


class ConstsContainer:
	def __init__(self):
		pass
	
	def __getattr__(self, attr):
		try:
			return self.__dict__[attr]
		except:
			raise AttributeError, "%s attribute does not exist yet" % attr
	
	def __setattr__(self, attr, value):
		if attr in self.__dict__:
			raise AttributeError, "Cannot reassign constant %s" % attr
		else:
			self.__dict__[attr] = value
	
	def __str__(self):
		return '\n'.join(['%s: %s' % (str(k), str(v)) for k, v in self.__dict__.items()])
	
	def __repr__(self):
		return '\n'.join(['%s: %s' % (str(k), str(v)) for k, v in self.__dict__.items()])
	
	def __iter__(self):
		return iter(self.__dict__.values())