def write_prescale_table(PStable, filepath='PStable_new', output_format='xlsx'):
    import pandas as pd

    if not filepath.endswith(output_format): filepath += '.' + output_format

    if isinstance(PStable, dict):
        # colnames = ['Name'].append([name for name in PStable.keys()])
        PStable = pd.DataFrame(PStable).transpose()

    PStable = PStable.fillna('-123')

    int_cols = [col for col in PStable.columns if PStable[col].dtype != str]
    for col in int_cols:
        PStable[col] = PStable[col].astype(int)

    PStable = PStable.replace(-123, '')

    PStable.index.rename('Name', inplace=True)

    if output_format in ['xlsx']:
        PStable.to_excel(filepath)
    else:
        raise NotImplementedError('Invalid output file format: {}'.format(
            output_format))
