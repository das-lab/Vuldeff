module Rgpg
  module GemInfo
    MAJOR_VERSION = 0
    MINOR_VERSION = 2
    PATCH_VERSION = 2

    def self.version_string
      [MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION].join('.')
    end
  end
end

