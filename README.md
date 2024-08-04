# What is this?

UbuntuBench_22.04 is an advanced benchmarking application based on the CIS Ubuntu Linux 22.04 LTS Benchmark v2.0.0 (03-28-2024). Designed to automate the testing of all CIS Benchmarks, this tool focuses on technical configuration settings critical for maintaining and enhancing the security of Ubuntu 22.04 LTS systems. It ensures compliance with industry standards by performing rigorous checks and validations.

# Key Features

- **Assessment Status:** Provides a comprehensive overview of the current security posture.

- **Rationale Statement:** Offers detailed explanations for each benchmark recommendation.

- **MITRE ATT&CK Mappings:** Links findings to the relevant MITRE ATT&CK techniques, enhancing threat intelligence.

- **Remediation Procedures:** Supplies step-by-step instructions for addressing identified issues.

# Intended Audience

This benchmark tool is intended for system and application administrators, security
specialists, auditors, help desk, and platform deployment personnel who plan to
develop, deploy, assess, or secure solutions that incorporate Ubuntu Linux 22.04 LTS
on x86_64 platforms.

## compilation

```bash
gcc -o ub_22 ub_22.c
```

```bash
./ub_22
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
