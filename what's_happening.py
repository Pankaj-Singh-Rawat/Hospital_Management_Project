# testing Hash
import hashlib
hash_object = hashlib.sha256()
# hash_object is hashed here already but to get its value for us to see
# we can do this!

hash_value = hash_object.hexdigest()
print("has_value of SHA-256 is:" , hash_value)