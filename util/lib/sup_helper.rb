module SupHelper
  def color(key, default = nil)
    c = fetch(key, default)
    if c
      c = c[1..-1]
      r = c[0, 2]
      g = c[2, 2]
      b = c[4, 2]
      [r, g, b].map{|s| s.to_i(16)}
    else
      nil
    end
  end

  def symbol(key, default = nil)
    sym = fetch(key, default)
    if sym.kind_of?(String)
      sym[0].ord
    elsif sym.kind_of?(Integer)
      sym
    else
      nil
    end
  end

  def integer(key, default = nil)
    num = fetch(key, default)
    if num
      num.to_i
    else
      nil
    end
  end

  def boolean(key)
    !!fetch(key)
  end
end
