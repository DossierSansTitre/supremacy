require 'fileutils'
require 'sup_file'

class Serializer
  @@serializers = {}

  def self.type(type)
    @@serializers[type] = self
  end

  def self.serialize_all(data_dir, binary_dir)
    dest_dir = File.join(binary_dir, 'data_unpacked')
    FileUtils.mkpath(dest_dir)
    sup_files = {}
    @@serializers.each do |type, _|
      source_path = File.join(data_dir, type.to_s)
      sup_files[type] = []
      Dir[File.join(source_path, '*.sup')].each {|path| sup_files[type] << SupFile.parse(type, path)}
    end
    @@serializers.each do |type, klass|
      serializer = klass.new
      data = []
      sup_files[type].sort_by!(&:id)
      sup_files[type].each {|sup| data << serializer.serialize(sup)}
      file_data = data.join('')
      File.open(File.join(dest_dir, type.to_s + '.bin'), "wb") do |file|
        file.write("SUP\x00")
        file.write([sup_files[type].size].pack("L<"))
        offset = 0
        sup_files[type].each_with_index do |sup, i|
          file.write([offset, data[i].size].pack('L<L<'))
          offset += data[i].size
        end
        file.write(file_data)
      end
    end
  end
end

require 'serializers/tile_serializer'
