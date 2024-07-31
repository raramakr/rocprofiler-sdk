# Samples

The samples are provided to help you see the profiler in action.

## Finding samples

After the ROCm build is installed:

- Sample programs are installed here:

    ```bash
    /opt/rocm/share/rocprofiler-sdk/samples
    ```
- `rocprofv3` tool is installed here:

    ```bash
    /opt/rocm/bin
    ```

## Building Samples

To build samples from any directory, run:

```bash
cmake -B build-rocprofiler-sdk-samples /opt/rocm/share/rocprofiler-sdk/samples -DCMAKE_PREFIX_PATH=/opt/rocm
cmake --build build-rocprofiler-sdk-samples --target all --parallel 8
```

## Running samples

To run the built samples, `cd` into the `build-rocprofiler-sdk-samples` directory and run:

```bash
ctest -V
```

:::{note}
Running a few of these tests require you to install Pandas and pytest first.
:::

```bash
/usr/local/bin/python -m pip install -r requirements.txt
```