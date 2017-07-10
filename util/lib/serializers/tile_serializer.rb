require 'serializer'

class TileSeriaslizer < Serializer
  type :tile

  def serialize(tile)
    symbol = tile.symbol('Symbol', ' ')
    symbol_dim = tile.symbol('SymbolDim', 0)
    drop_frequency = tile.integer('DropFrequency', 0)
    walkable = tile.boolean('Walkable') ? 1 : 0
    [symbol, symbol_dim, drop_frequency, walkable].pack('S<S<CC')
  end
end
