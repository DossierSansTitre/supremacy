class BiomeSerializer < Serializer
  type :biome

  # Not converted to the new system since
  # this will be refactored a lot
  def serialize(biome)
    name = biome.fetch('DisplayName', biome.name)
    color = biome.color('Color', '#000000')
    bg_color = biome.color('BackgroundColor', '#000000')
    symbol = biome.symbol('Symbol', 0)

    height = biome.fetch('Height', '0 0').split(' ').map{|x| x.strip.to_i}
    temperature = biome.fetch('Temperature', '0 1000').split(' ').map{|x| x.strip.to_i}
    rain = biome.fetch('Rain', '0 1000').split(' ').map{|x| x.strip.to_i}
    drainage = biome.fetch('Drainage', '0 1000').split(' ').map{|x| x.strip.to_i}

    layer_top = biome.ref(:material, 'LayerTop', 'Air')
    layer_middle = biome.ref(:material, 'LayerMiddle', 'Air')
    layer_bottom = biome.ref(:material, 'LayerBottom', 'Air')

    tree_density = biome.fetch('TreeDensity', '0').to_i
    tree_tile, tree_mat = tile_pair(biome.fetch('TreeType', 'Nothing:Air'))

    flags = 0
    flags |= (biome.boolean('NoEmbark') ? 0x01 : 0)

    emit(biome.id, 'S<')
    emit([name.size, name], 'S<a*')
    emit(color, 'C3')
    emit(bg_color, 'C3')
    emit(symbol, 'S<')

    emit(height, 'S<')
    emit(temperature, 'S<')
    emit(rain, 'S<')
    emit(drainage, 'S<')

    emit(layer_top, 'S<')
    emit(layer_middle, 'S<')
    emit(layer_bottom, 'S<')

    emit(tree_density, 'S<')
    emit(tree_tile, 'S<')
    emit(tree_mat, 'S<')

    emit(flags, 'S<')
  end

  def tile_pair(value)
    tile, mat = value.split(':')
    [SupFile.reference(:tile, tile), SupFile.reference(:material, mat)]
  end
end
