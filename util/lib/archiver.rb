require 'fileutils'

module Archiver
  def self.run(binary_dir)
    FileUtils.mkpath(File.join(binary_dir, 'data'))
    files = {}
    Dir.chdir(File.join(binary_dir, 'data_unpacked')) do
      f = Dir['**/*'].reject{|path| path == '.' || path == '..'}
      f.each {|name| files[name] = File.binread(name)}
    end
    File.open(File.join(binary_dir, 'supremacy.bin'), "wb") do |file|
      file.write("SUPA")
      file.write([files.size].pack("L<"))
      offset = 0
      files.each do |name, data|
        file.write([name.size, name, offset, data.size].pack("S<a*L<L<"))
        offset += data.size
      end
      files.each do |_, data|
        file.write(data)
      end
    end
  end
end
