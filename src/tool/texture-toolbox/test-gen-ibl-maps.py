"""Regression coverage for DDS cubemap serialization."""

import importlib.util
import pathlib
import struct
import tempfile
import unittest

import numpy as np


class CubemapWriterTest(unittest.TestCase):
    def test_each_face_contains_its_complete_mip_chain(self):
        spec = importlib.util.spec_from_file_location(
            "gen_ibl_maps", pathlib.Path(__file__).with_name("gen-ibl-maps.py")
        )
        generator = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(generator)
        mips = [
            [np.full((2, 2, 4), face + 1, dtype=np.float32) for face in range(6)],
            [np.full((1, 1, 4), face + 11, dtype=np.float32) for face in range(6)],
        ]
        with tempfile.TemporaryDirectory() as directory:
            for fmt, dtype in (("rgba16f", "<f2"), ("rgba32f", "<f4")):
                with self.subTest(format=fmt):
                    path = pathlib.Path(directory) / "cube.dds"
                    generator.write_dds_cubemap(mips, str(path), fmt)
                    data = path.read_bytes()
                    self.assertEqual(data[:4], b"DDS ")
                    self.assertEqual(struct.unpack_from("<I", data, 28)[0], 2)
                    pixels = np.frombuffer(data, dtype=dtype, offset=148)
                    self.assertEqual(pixels.size, 6 * (16 + 4))
                    for face in range(6):
                        # DDS array slices are outermost: 4 base texels followed
                        # by the one mip texel, before any data from the next face.
                        start = face * 20
                        np.testing.assert_array_equal(
                            pixels[start : start + 16], face + 1
                        )
                        np.testing.assert_array_equal(
                            pixels[start + 16 : start + 20], face + 11
                        )


if __name__ == "__main__":
    unittest.main()
