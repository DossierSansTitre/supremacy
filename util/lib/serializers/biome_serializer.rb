class BiomeSerializer < Serializer
  type :biome

  def serialize(biome)
    layers = biome.fetch('Layers', []).map{|layer| serialize_layer(biome, layer)}
    layers_data = [layers.size].pack('S<') + layers.join('')
    color = biome.color('Color', '#000000')
    bg_color = biome.color('BackgroundColor', '#000000')
    symbol = biome.symbol('Symbol', 0)
    [biome.id].pack('S<') + layers_data + [color, bg_color, symbol].flatten.pack('C6S<')
  end

  private
  def serialize_layer(biome, layer)
    params = layer.strip.split(' ')
    func = params.shift
    func_num = nil
    func_params = nil
    case func
    when 'FILL'
      n = params.shift.to_i
      tile, mat = tile_pair(params.shift)
      func_num = 1
      func_params = [n, tile, mat].pack('S<S<S<')
    when 'OCTAVE'
      persistance = params.shift.to_f
      octaves = params.shift.to_i
      power = params.shift.to_f
      a = params.shift.to_i
      b = params.shift.to_i
      tile, mat = tile_pair(params.shift)
      func_num = 2
      func_params = [persistance, octaves, power, a, b, tile, mat].pack('eS<eS<S<S<S<')
    when 'SMOOTH'
      from = SupFile.reference(:tile, params.shift)
      to = SupFile.reference(:tile, params.shift)
      func_num = 3
      func_params = [from, to].pack('S<S<')
    when 'REPLACE'
      from_tile, from_material = tile_pair(params.shift)
      to_tile, to_material = tile_pair(params.shift)
      frequency = params.shift.to_i
      func_num = 4
      func_params = [from_tile, from_material, to_tile, to_material, frequency].pack('S<4L<')
    end
    [func_num].pack('S<') + func_params
  end

  def tile_pair(value)
    tile, mat = value.split(':')
    [SupFile.reference(:tile, tile), SupFile.reference(:material, mat)]
  end
end
