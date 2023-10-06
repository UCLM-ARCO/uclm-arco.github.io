module st {

  // basic types
  interface IBool    { void set(bool v, string sourceAddr); };
  interface IByte    { void set(byte v, string sourceAddr); };
  interface IFloat   { void set(float v, string sourceAddr); };
  interface IString  { void set(string v, string sourceAddr); };

  // observable pattern
  interface Linkable { void linkto(string observerAddr); };
};

