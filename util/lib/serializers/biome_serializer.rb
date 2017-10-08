class BiomeSerializer < Serializer
  type :biome

  # Not converted to the new system since
  # this will be refactored a lot
  def serialize(biome)
    name = biome.fetch('DisplayName', biome.name)
    layers = biome.fetch('Layers', []).map{|layer| serialize_layer(biome, layer)}
    layers_data = [layers.size].pack('S<') + layers.join('')
    color = biome.color('Color', '#000000')
    bg_color = biome.color('BackgroundColor', '#000000')
    symbol = biome.symbol('Symbol', 0)
    height = biome.fetch('Height', '0 0').split(' ').map{|x| x.strip.to_i}
    temperature = biome.fetch('Temperature', '0 1000').split(' ').map{|x| x.strip.to_i}
    rain = biome.fetch('Rain', '0 1000').split(' ').map{|x| x.strip.to_i}
    drainage = biome.fetch('Drainage', '0 1000').split(' ').map{|x| x.strip.to_i}
    tree_density = biome.fetch('TreeDensity', '0').to_i
    tree_tile, tree_mat = tile_pair(biome.fetch('TreeType', 'Nothing:Air'))
    flags = 0
    flags |= (biome.boolean('NoEmbark') ? 0x01 : 0)
    data = [biome.id].pack('S<') + [name.size, name].pack('S<a*') + layers_data + [color, bg_color, symbol, height, temperature, rain, drainage, tree_density, tree_tile, tree_mat, flags].flatten.pack('C6S<S<S<S<S<S<S<S<S<S<CCC')
    emit(data, 'a*')
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
