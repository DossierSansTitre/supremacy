require 'serializer'

class TileSeriaslizer < Serializer
  type :tile

  def serialize(tile)
    symbol = tile.symbol('Symbol', ' ')
    symbol_dim = tile.symbol('SymbolDim', 0)
    drop_frequency = tile.integer('DropFrequency', 0)
    flags = 0
    flags |= (tile.boolean('Walkable') ? 0x01 : 0)
    flags |= (tile.boolean('Block') ? 0x02 : 0)
    flags |= (tile.boolean('Support') ? 0x04 : 0)
    flags |= (tile.boolean('MoveUp') ? 0x08 : 0)
    flags |= (tile.boolean('MoveDown') ? 0x10 : 0)

    emit(tile.id, 'S<')
    emit(symbol, 'S<')
    emit(symbol_dim, 'S<')
    emit(drop_frequency, 'S<')
    emit(flags, 'C')
  end
end
