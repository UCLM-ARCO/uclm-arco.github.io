
import sys


class MyUtils:
    """
    This is a basic configurable calculator. It enables or disables the `sum`
    operation in yor math space.
    """

    def __init__(self, args):
        """"""  # private constructor, avoid documenting it
        self.args = args

    def division(self, a, b):
        # work in progress
        pass

    def sum(self, a, b):
        """
        Returns the addition of both arguments, if addition is a valid
        operation.

        **Parameters:**

        * `a`: first operand for the addition. It must be an **integer** or a
          **float**.
        * `b`: second operand for the addition, of same type that `a` param.
        """
        assert self.can_sum
        return a + b

    @property
    def can_sum(self):
        """
        *[read only]* Property to check whether the `sum` operation could be
        done.
        """
        return "sum" in self.args


class MyTools:

    def print(self, msg):
        # work in progress
        pass