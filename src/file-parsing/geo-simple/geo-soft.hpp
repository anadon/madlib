#ifndef GEO_SOFT_HPP
#define GEO_SOFT_HPP

#include<string>

/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
const char *CSTRING_PLATFORM = "^PLATFORM";
static const std::string STRING_PLATFORM(CSTRING_PLATFORM);
//std::string platform;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-120 characters, must be
unique within local file and over all previously submitted Platforms for that
submitter

Content guidelines: Provide a unique title that describes your Platform. We
suggest that you use the system
[institution/lab]-[species]-[number of features]-[version], e.g.
"FHCRC Mouse 15K v1.0".
*******************************************************************************/
const char* CSTRING_Platform_title = "!Platform_title";
static const std::string STRING_Platform_title(CSTRING_Platform_title);
//std::string platform_title;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: commercial, non-commercial, custom-commercial,
or virtual

Content guidelines: Microarrays are 'commercial', 'non-commercial', or
'custom-commercial' in accordance with how the array was manufactured. Use
'virtual' only if creating a virtual definition for MPSS, SARST, or RT-PCR data.
*******************************************************************************/
const char* CSTRING_Platform_distribution = "!Platform_distribution";
static const std::string STRING_Platform_distribution(CSTRING_Platform_distribution);
enum platform_distribution_enum{
  commercial, non_commercial, custom_commercial, _virtual,
  platform_distribution_unset
};


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: spotted DNA/cDNA, spotted oligonucleotide,
in situ oligonucleotide, antibody, tissue, SARST, RT-PCR, or MPSS

Content guidelines: Select the category that best describes the Platform
technology.
*******************************************************************************/
const char* CSTRING_Platform_technology = "!Platform_technology";
static const std::string STRING_Platform_technology(CSTRING_Platform_technology);
enum platform_technology_enum{
  spotted_DNA_or_cDNA, spotted_oligonucleotide, in_situ_oligonucleotide,
  antibody, tissue, SARST, RT_PCR, MPSS, platform_technology_unset
};


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: use standard NCBI Taxonomy nomenclature

Content guidelines: Identify the organism(s) from which the features on the
Platform were designed or derived.
*******************************************************************************/
const char* CSTRING_Platform_organism = "!Platform_organism";
static const std::string STRING_Platform_organism(CSTRING_Platform_organism);
//std::vector<std::string> platform_organism;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any

Content guidelines: Provide the name of the company, facility or laboratory
where the array was manufactured or produced.
*******************************************************************************/
const char* CSTRING_Platform_manufacturer = "!Platform_manufacturer";
static const std::string STRING_Platform_manufacturer(CSTRING_Platform_manufacturer);
//std::string platform_manufacturer;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the array manufacture protocol. Include as much
detail as possible, e.g., clone/primer set identification and preparation,
strandedness/length, arrayer hardware/software, spotting protocols. You can
include as much text as you need to thoroughly describe the protocol; it is
strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
const char* CSTRING_Platform_manufacture_protocol = "!Platform_manufacture_protocol";
static const std::string STRING_Platform_manufacture_protocol(CSTRING_Platform_manufacture_protocol);
//std::vector<std::string> platform_manufacture_protocol;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Provide the manufacturer catalog number for commercially-
available arrays.
*******************************************************************************/
const char* CSTRING_Platform_catalog_number = "!Platform_catalog_number";
static const std::string STRING_Platform_catalog_number(CSTRING_Platform_catalog_number);
//std::vector<std::string> platform_catalog_number;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: valid URL

Content guidelines: Specify a Web link that directs users to supplementary
information about the array. Please restrict to Web sites that you know are
stable.
*******************************************************************************/
const char* CSTRING_Platform_web_link = "!Platform_web_link";
static const std::string STRING_Platform_web_link(CSTRING_Platform_web_link);
//std::vector<std::string> platform_web_link;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: any

Content guidelines: Provide the surface type of the array, e.g., glass,
nitrocellulose, nylon, silicon, unknown.
*******************************************************************************/
const char* CSTRING_Platform_support = "!Platform_support";
static const std::string STRING_Platform_support(CSTRING_Platform_support);
//std::string platform_support;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: any

Content guidelines: Provide the coating of the array, e.g., aminosilane, quartz,
polysine, unknown.
*******************************************************************************/
const char* CSTRING_Platform_coating = "!Platform_coating";
static const std::string STRING_Platform_coating(CSTRING_Platform_coating);
//std::string platform_coating;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Provide any additional descriptive information not captured
in another field, e.g., array and/or feature physical dimensions, element grid
system.
*******************************************************************************/
const char* CSTRING_Platform_description = "!Platform_description";
static const std::string STRING_Platform_description(CSTRING_Platform_description);
//std::string platform_description;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value in the form
'firstname,middleinitial,lastname' or 'firstname,lastname': firstname must be at
least one character and cannot contain spaces; middleinitial, if present, is one
character; lastname is at least two characters and can contain spaces.

Content guidelines: List all people associated with this array design.
*******************************************************************************/
const char* CSTRING_Platform_contributor = "!Platform_contributor";
static const std::string STRING_Platform_contributor(CSTRING_Platform_contributor);
//std::vector<std::string> platform_contributor;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: an integer

Content guidelines: Specify a valid PubMed identifier (PMID) that references a
published article that describes the array.
*******************************************************************************/
const char* CSTRING_Platform_pubmed_id = "!Platform_pubmed_id";
static const std::string STRING_Platform_pubmed_id(CSTRING_Platform_pubmed_id);
//std::vector<int> platform_pubmed_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Platform accession number (GPLxxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
const char* CSTRING_Platform_geo_accession = "!Platform_geo_accession";
static const std::string STRING_Platform_geo_accession(CSTRING_Platform_geo_accession);
//std::string platform_geo_accession;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the start of the data table.  Data cells can be
restricted to certain types if a standard column header is used, but other
column headers may be used and be tolerated.

TODO: more in depth parsing, validation, and specific representation of cells.

Standard Headers:

TAG: ID
Datatype: std::string
Regex: *
Qualifications: unique in column, required, unique among column headers
Details: An identifier that unambiguously identifies each row on your Platform
table. Each ID within a Platform table must be unique. This column heading
should appear first and may be used only once in the data table. Keep in mind
that the ID column you provide in your Platform data table corresponds with the
ID_REF column you provide in accompanying Sample data tables. Sample data tables
should contain normalized data. If your normalization strategy requires taking
the average of replicate array features, your Platform should reflect the
condensed template. In this case, please e-mail or FTP the full template file to
us and we will attach it to your Platform record as a supplementary file.

TAG: SEQUENCE
Datatype: std::string
Regex: [ATCG]+
Qualifications: unique among column headers
Details: The nucleotide sequence of each oligo, clone or PCR product.

TAG: GB_ACC
Datatype: std::string
Regex: .+\.[0-9]+
Qualifications:
Details: GenBank accession - identifies a biological sequence through the GenBank
sequence accession number assigned to the sequence, or the representative
GenBank or RefSeq accession number upon which your sequence was designed. It is
recommended that you include the version number of the accessions upon which
your sequences were designed (e.g., NM_022975.1 rather than NM_022975). This is
particularly important for RefSeq accessions which are updated frequently.
GenBank accessions representing the top BLAST hits for your sequences are not
acceptable. Also, chromosome, genome and contig accession numbers are generally
not acceptable as they are not specific enough to accurately identify the
portion of the sequence printed on arrays (use GB_RANGE instead).

TAG: GB_LIST
Datatype: std::string
Regex: *
Qualifications:
Details: GenBank accession list - as for GB_ACC, but allows more than one
GenBank accession number to be presented. For example, your sequences may have
GenBank accession numbers representing both the 5' and 3' ends of your clones.
Multiple accession numbers should be separated using commas or spaces.
Alternatively, more than one GB_ACC column may be supplied.

TAG: GB_RANGE
Datatype: std::string
Regex: .+\.[0-9]+\[[0-9]+\.\.[0-9]+\]
Qualifications: start < end
Details: GenBank accession range - specifies a particular sequence position
within a GenBank accession number. Use format ACCESSION.VERSION[start..end].
Useful for tiling arrays.

TAG: RANGE_GB
Datatype: std::string
Regex: .+\.[0-9]+
Qualifications:
Details: Use format ACCESSION.VERSION. Should be used in conjunction with
RANGE_START and RANGE_END. Useful for tiling arrays.

TAG: RANGE_START
Datatype: long int
Regex: [0-9]+
Qualifications: Implicitly requires RANGE_END, less than RANGE_END
Details: Use in conjunction with RANGE_GB. Indicates the start position
(relative to the RANGE_GB accession). Useful for tiling arrays.

TAG: RANGE_END
Datatype: long int
Regex: [0-9]+
Qualifications: Implicitly requires RANGE_START, greater than RANGE_START
Details: Use in conjunction with RANGE_GB. Indicates the end position
(relative to the RANGE_GB accession). Useful for tiling arrays.

TAG: RANGE_STRAND
Datatype: std::string
Regex: {\+}|{\-}|{}|{empty}
Qualifications:
Details: Use in conjunction with RANGE_GB. Indicates the strand represented. Use
+ or - or empty. Useful for tiling arrays.

TAG: GI
Datatype: std::string
Regex: .+
Qualifications:
Details: GenBank identifier - as for GB_ACC, but specify the GenBank identifier
number rather than the GenBank accession number.

TAG: GI_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: GenBank identifier list - as for GI, but allows more than one GenBank
identifier to be presented. Multiple GIs should be separated using commas or
spaces. Alternatively, more than one GI column may be supplied.

TAG: GI_RANGE
Datatype: std::string
Regex: GI\[[0-9]+\.\.[0-9]+\]
Qualifications: First number is less than second
Details: GenBank identifier range - specifies a particular sequence position on
a GenBank identifier number. Use format GI[start..end].

TAG: CLONE_ID
Datatype: std::string
Regex: .+
Qualifications:
Details: Clone identifier - identifies a biological sequence through a standard
clone identifier. Only CLONE_IDs that can be used to identify the sequence
through an NCBI or other public-database query should be provided in this
column. Examples include FlyBase IDs, RIKEN clone IDs and IMAGE clone numbers.

TAG: CLONE_ID_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: CLONE_ID list - as for CLONE_ID, but allows more than one clone
identifier to be presented. Multiple Clone IDs should be separated using commas
or spaces. Alternatively, more than one CLONE_ID column may be supplied.

TAG: ORF
Datatype: std::string
Regex: .+
Qualifications:
Details: Open reading frame designator - identifies a biological sequence
through an experimentally or computationally derived open reading frame
identifier. The ORF designator is intended to represent a known or predicted DNA
coding region or locus_tag identified in NCBI's Entrez Genomes division. It may
be appropriate to include a GENOME_ACC column to reference the GenBank accession
from which the ORF names are derived.

TAG: ORF_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: ORF list - as for ORF, but allows more than one open reading frame
designator to be presented. Multiple ORFs should be separated using commas or
spaces. Alternatively, more than one ORF column may be supplied.

TAG: GENOME_ACC
Datatype: std::string
Regex: .+\.[0-9]+
Qualifications:
Details: Genome accession number - specifies the GenBank or RefSeq genome
accession number from which ORF identifiers are derived. It is important to
include the version number of the genome accession upon which your sequences
were generated (e.g., NC_004721.1 rather than NC_004721) because updates to the
genome sequence may render your ORF designations incorrect.

TAG: SNP_ID
Datatype: std::string
Regex: .+
Qualifications:
Details: SNP identifier - typically specifies a dbSNP refSNP ID with format
rsXXXXXXXX.

TAG: SNP_ID_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: SNP identifier list - as for SNP_ID, but allows more than one
SNP_ID to be presented. Multiple SNP_IDs should be separated using commas or
spaces. Alternatively, more than one SNP_ID column may be supplied.

TAG: miRNA_ID
Datatype: std::string
Regex: .+
Qualifications:
Details: microRNA identifier - typically has format e.g., hsa-let-7a or
MIRNLET7A2.

TAG: miRNA_ID_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: microRNA identifier list - as for miRNA_ID, but allows more than
one miRNA_ID to be presented. Multiple miRNA_IDs should be separated using
commas or spaces. Alternatively, more than one miRNA_ID column may be supplied.

TAG: SPOT_ID
Datatype: std::string
Regex: .+
Qualifications:
Details: Alternative spot identifier - use only when no identifier or sequence
tracking information is available. This column is useful for designating control
and empty features.

TAG: ORGANISM
Datatype: std::string
Regex: .+
Qualifications: unique among column headers
Details: The organism source of each feature on your array. This is most useful
for when your array contains sequences derived from multiple organisms.

TAG: PT_ACC
Datatype: std::string
Regex: .+
Qualifications:
Details: Protein accession - identifies any GenBank or RefSeq protein accession
number. Protein accession numbers should only be supplied for protein arrays.
Nucleotide accession numbers should be supplied for nucleotide arrays.
PT_LIST: Protein accession list - as for PT_ACC, but allows more than one
protein accession number to be presented. Multiple accession numbers should be
separated using commas or spaces. Alternatively, more than one PT_ACC column may
be supplied. Protein accession numbers should only be supplied for protein
arrays. Nucleotide accession numbers should be supplied for nucleotide arrays.

TAG: PT_GI
Datatype: std::string
Regex: .+
Qualifications:
Details: Protein GenBank or RefSeq identifier. Protein identifiers should only be
supplied for protein arrays or proteomic mass spectrometry Platforms. Nucleotide
identifiers should be supplied for nucleotide arrays.

TAG: PT_GI_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: Protein identifier list - as for PT_GI, but allows more than one
protein identifier to be presented. Multiple identifiers should be separated
using commas or spaces. Alternatively, more than one PT_GI column may be
supplied. Protein identifiers should only be supplied for protein arrays.
Nucleotide identifiers should be supplied for nucleotide arrays.

TAG: SP_ACC
Datatype: std::string
Regex: .+
Qualifications:
Details: SwissProt accession. SwissProt accession numbers should only be supplied
for protein arrays. Nucleotide accession numbers should be supplied for
nucleotide arrays.

TAG: SP_LIST
Datatype: std::string
Regex: .+
Qualifications:
Details: SwissProt accession list - as for SP_ACC, but allows more than one
SwissProt accession number to be presented. Multiple accession numbers should be
separated using commas or spaces. Alternatively, more than one SP_ACC column may
be supplied. SwissProt accession numbers should only be supplied for protein
arrays. Nucleotide accession numbers should be supplied for nucleotide arrays.

*******************************************************************************/
const char* CSTRING_Platform_table_begin = "!Platform_table_begin";
static const std::string STRING_Platform_table_begin(CSTRING_Platform_table_begin);
//std::vector<std::string> platform_table_column_titles;
//std::vector<std::vector<std::string> > platform_table_matrix;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the end of the data table.
*******************************************************************************/
const char* CSTRING_Platform_table_end = "!Platform_table_end";
static const std::string STRING_Platform_table_end(CSTRING_Platform_table_end);


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
const char* CSTRING_SAMPLE = "^SAMPLE";
static const std::string STRING_SAMPLE(CSTRING_SAMPLE);
//std::string sample;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-120 characters, must be
unique within local file and over all previously submitted Samples for that
submitter

Content guidelines: Provide a unique title that describes this Sample. We
suggest that you use the system [biomaterial]-[condition(s)]-[replicate number],
e.g., Muscle_exercised_60min_rep2.
*******************************************************************************/
const char* CSTRING_Sample_title = "!Sample_title";
static const std::string STRING_Sample_title(CSTRING_Sample_title);
//std::string sample_title;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: name of supplementary file, or 'none'

Content guidelines: Examples of supplementary file types include original
Affymetrix CEL and EXP files, GenePix GPR files, and TIFF image files.
Supplementary files should be zipped or tarred together with the SOFT file at
time of submission (do not include any sub-directories or sub-folders in your
zip/tar archive). Provision of supplementary raw data files facilitates the
unambiguous interpretation of data and potential verification of conclusions as
set forth in the MIAME guidelines.
*******************************************************************************/
const char* CSTRING_Sample_supplementary_file = "!Sample_supplementary_file";
static const std::string STRING_Sample_supplementary_file(CSTRING_Sample_supplementary_file);
//std::vector<std::string> sample_supplementary_file;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: name of external CHP or tab-delimited file to be
used as data table

Content guidelines: - Affymetrix CHP file name:
If your processed data are CHP files, you can reference the CHP file name in
this field. If your manuscript discusses data processed by RMA or another
algorithm, we recommend providing those values in the table section. There is no
need to specify the !Sample_platform_id when CHP files are supplied. All
external files should be zipped or tarred together with the SOFT file at time of
submission.
- Tab-delimited table file name:
If it is convenient for you to generate, you can reference the name of an
external tab-delimited table file (see format) in this field, rather than
include the table in the !Sample_table_begin section. All external files should
be zipped or tarred together with the SOFT file at time of submission.
*******************************************************************************/
const char* CSTRING_Sample_table = "!Sample_table";
static const std::string STRING_Sample_table(CSTRING_Sample_table);
//std::string sample_table;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: any

Content guidelines: Briefly identify the biological material and the
experimental variable(s), e.g., vastus lateralis muscle, exercised, 60 min.
*******************************************************************************/
const char* CSTRING_Sample_source_name_ch = "!Sample_source_name_ch";
static const std::string STRING_Sample_source_name_ch(CSTRING_Sample_source_name_ch);
//std::vector<std::string> sample_source_name;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: use standard NCBI Taxonomy nomenclature

Content guidelines: Identify the organism(s) from which the biological material
was derived.
*******************************************************************************/
const char* CSTRING_Sample_organism_ch = "!Sample_organism_ch";
static const std::string STRING_Sample_organism_ch(CSTRING_Sample_organism_ch);
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_organism;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: 'Tag: Value' format

Content guidelines: Describe all available characteristics of the biological
source, including factors not necessarily under investigation. Provide in
'Tag: Value' format, where 'Tag' is a type of characteristic (e.g. "gender",
"strain", "tissue", "developmental stage", "tumor stage", etc), and 'Value' is
the value for each tag (e.g. "female", "129SV", "brain", "embryo", etc). Include
as many characteristics fields as necessary to thoroughly describe your Samples.
*******************************************************************************/
const char* CSTRING_Sample_characteristics_ch = "!Sample_characteristics_ch";
static const std::string STRING_Sample_characteristics_ch(CSTRING_Sample_characteristics_ch);
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::pair<std::string, std::string> > >
//sample_characteristics;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Specify the name of the company, laboratory or person that
provided the biological material.
*******************************************************************************/
const char* CSTRING_Sample_biomaterial_provider_ch = "!Sample_biomaterial_provider_ch";
static const std::string STRING_Sample_biomaterial_provider_ch(CSTRING_Sample_biomaterial_provider_ch);
//The number of allowed labels is inaccurate; it means 0 or more per channel
//std::vector<std::vector<std::string> > sample_biomaterial_provider;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe any treatments applied to the biological material
prior to extract preparation. You can include as much text as you need to
thoroughly describe the protocol; it is strongly recommended that complete
protocol descriptions are provided within your submission.
*******************************************************************************/
const char* CSTRING_Sample_treatment_protocol_ch = "!Sample_treatment_protocol_ch";
static const std::string STRING_Sample_treatment_protocol_ch(CSTRING_Sample_treatment_protocol_ch);
//The number of allowed labels is inaccurate; it means 0 or more per channel
//std::vector<std::vector<std::string> > sample_treatment_protocol;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe the conditions that were used to grow or maintain
organisms or cells prior to extract preparation. You can include as much text as
you need to thoroughly describe the protocol; it is strongly recommended that
complete protocol descriptions are provided within your submission.
*******************************************************************************/
const char* CSTRING_Sample_growth_protocol_ch = "!Sample_growth_protocol_ch";
static const std::string STRING_Sample_growth_protocol_ch(CSTRING_Sample_growth_protocol_ch);
//The number of allowed labels is inaccurate; it means 0 or more per channel
//std::vector<std::vector<std::string> > sample_growth_protocol;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: total RNA, polyA RNA, cytoplasmic RNA, nuclear
RNA, genomic DNA, protein, or other

Content guidelines: Specify the type of molecule that was extracted from the
biological material.
*******************************************************************************/
const char* CSTRING_Sample_molecule_ch = "!Sample_molecule_ch";
static const std::string STRING_Sample_molecule_ch(CSTRING_Sample_molecule_ch);
//std::vector<std::string> sample_molecule;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocol used to isolate the extract material.
You can include as much text as you need to thoroughly describe the protocol; it
is strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
const char* CSTRING_Sample_extract_protocol_ch = "!Sample_extract_protocol_ch";
static const std::string STRING_Sample_extract_protocol_ch(CSTRING_Sample_extract_protocol_ch);
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_extract_protocol;


/*******************************************************************************
Number of allowed labels: 1 per channel

Allowed values and constraints: any

Content guidelines: Specify the compound used to label the extract e.g., biotin,
Cy3, Cy5, 33P.
*******************************************************************************/
const char* CSTRING_Sample_label_ch = "!Sample_label_ch";
static const std::string STRING_Sample_label_ch(CSTRING_Sample_label_ch);
//std::vector<std::string> sample_label;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocol used to label the extract. You can
include as much text as you need to thoroughly describe the protocol; it is
strongly recommended that complete protocol descriptions are provided within
your submission.
*******************************************************************************/
const char* CSTRING_Sample_label_protocol_ch = "!Sample_label_protocol_ch";
static const std::string STRING_Sample_label_protocol_ch(CSTRING_Sample_label_protocol_ch);
//The number of allowed labels is inaccurate; it means 1 or more per channel
//std::vector<std::vector<std::string> > sample_label_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the protocols used for hybridization, blocking and
washing, and any post-processing steps such as staining. You can include as much
text as you need to thoroughly describe the protocol; it is strongly recommended
that complete protocol descriptions are provided within your submission.
*******************************************************************************/
const char* CSTRING_Sample_hyb_protocol = "!Sample_hyb_protocol";
static const std::string STRING_Sample_hyb_protocol(CSTRING_Sample_hyb_protocol);
//std::vector<std::string> sample_hyb_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Describe the scanning and image acquisition protocols,
hardware, and software. You can include as much text as you need to thoroughly
describe the protocol; it is strongly recommended that complete protocol
descriptions are provided within your submission.
*******************************************************************************/
const char* CSTRING_Sample_scan_protocol = "!Sample_scan_protocol";
static const std::string STRING_Sample_scan_protocol(CSTRING_Sample_scan_protocol);
//std::vector<std::string> sample_scan_protocol;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Provide details of how data in the VALUE column of your
table were generated and calculated, i.e., normalization method, data selection
procedures and parameters, transformation algorithm (e.g., MAS5.0), and scaling
parameters. You can include as much text as you need to thoroughly describe the
processing procedures.
*******************************************************************************/
const char* CSTRING_Sample_data_processing = "!Sample_data_processing";
static const std::string STRING_Sample_data_processing(CSTRING_Sample_data_processing);
//std::vector<std::string> sample_data_processing;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Include any additional information not provided in the other
fields, or paste in broad descriptions that cannot be easily dissected into the
other fields.
*******************************************************************************/
const char* CSTRING_Sample_description = "!Sample_description";
static const std::string STRING_Sample_description(CSTRING_Sample_description);
//std::vector<std::string> sample_description;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: a valid Platform identifier

Content guidelines: Reference the Platform upon which this hybridization was
performed. Reference the Platform accession number (GPLxxx) if the Platform
already exists in GEO, or reference the ^Platform identifier if the Platform
record is being batch submitted within the same SOFT file. To identify the
accession number of an existing commercial Platform in GEO, use the FIND
PLATFORM tool.
*******************************************************************************/
const char* CSTRING_Sample_platform_id = "!Sample_platform_id";
static const std::string STRING_Sample_platform_id(CSTRING_Sample_platform_id);
//std::string sample_platform_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Sample accession number (GSMxxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
const char* CSTRING_Sample_geo_accession = "!Sample_geo_accession";
static const std::string STRING_Sample_geo_accession(CSTRING_Sample_geo_accession);
//std::string sample_geo_accession;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: SAGE enzyme anchor, usually NlaIII or Sau3A

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
const char* CSTRING_Sample_anchor = "!Sample_anchor";
static const std::string STRING_Sample_anchor(CSTRING_Sample_anchor);
//std::string sample_anchor;


/*******************************************************************************
Number of allowed labels:  1

Allowed values and constraints: SAGE

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
const char* CSTRING_Sample_type = "!Sample_type";
static const std::string STRING_Sample_type(CSTRING_Sample_type);
//std::string sample_type;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: sum of tags quantified in SAGE library

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
const char* CSTRING_Sample_tag_count = "!Sample_tag_count";
static const std::string STRING_Sample_tag_count(CSTRING_Sample_tag_count);
//int sample_tag_count;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: base pair length of the SAGE tags, excluding
anchor sequence

Content guidelines: Use for SAGE submissions only.
*******************************************************************************/
const char* CSTRING_Sample_tag_length = "!Sample_tag_length";
static const std::string STRING_Sample_tag_length(CSTRING_Sample_tag_length);
//int sample_tag_length;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the start of the data table.
*******************************************************************************/
const char* CSTRING_Sample_table_begin = "!Sample_table_begin";
static const std::string STRING_Sample_table_begin(CSTRING_Sample_table_begin);
//std::vector<std::string> sample_table_columns;
//std::vector<std::vector<std::string> > sample_table_matrix;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: no content required

Content guidelines: Indicates the end of the data table.
*******************************************************************************/
const char* CSTRING_Sample_table_end = "!Sample_table_end";
static const std::string STRING_Sample_table_end(CSTRING_Sample_table_end);


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any, must be unique within local file

Content guidelines: Provide an identifier for this entity. This identifier is
used only as an internal reference within a given file. The identifier will not
appear on final GEO records.
*******************************************************************************/
const char* CSTRING_SERIES = "^SERIES";
static const std::string STRING_SERIES(CSTRING_SERIES);
//std::string series;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: string of length 1-255 characters, must be
unique within local file and over all previously submitted Series for that
submitter

Content guidelines: string of length 1-255 characters, must be unique within
local file and over all previously submitted Series for that submitter
*******************************************************************************/
const char* CSTRING_Series_title = "!Series_title";
static const std::string STRING_Series_title(CSTRING_Series_title);
//std::string series_title;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: any

Content guidelines: Summarize the goals and objectives of this study. The
abstract from the associated publication may be suitable. You can include as
much text as you need to thoroughly describe the study.
*******************************************************************************/
const char* CSTRING_Series_summary = "!Series_summary";
static const std::string STRING_Series_summary(CSTRING_Series_summary);
//std::vector<std::string> series_summary;


/*******************************************************************************
Number of allowed labels: 1

Allowed values and constraints: any

Content guidelines: Provide a description of the experimental design. Indicate
how many Samples are analyzed, if replicates are included, are there control
and/or reference Samples, dye-swaps, etc.
*******************************************************************************/
const char* CSTRING_Series_overall_design = "!Series_overall_design";
static const std::string STRING_Series_overall_design(CSTRING_Series_overall_design);
//std::string series_overall_design;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: an integer

Content guidelines: Specify a valid PubMed identifier (PMID) that references a
published article describing this study. Most commonly, this information is not
available at the time of submission - it can be added later once the data are
published.
*******************************************************************************/
const char* CSTRING_Series_pubmed_id = "!Series_pubmed_id";
static const std::string STRING_Series_pubmed_id(CSTRING_Series_pubmed_id);
//std::vector<std::string> series_pubmed_id;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: valid URL

Content guidelines: Specify a Web link that directs users to supplementary
information about the study. Please restrict to Web sites that you know are
stable.
*******************************************************************************/
const char* CSTRING_Series_web_link = "!Series_web_link";
static const std::string STRING_Series_web_link(CSTRING_Series_web_link);
//std::vector<std::string> series_web_link;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: 'firstname,middleinitial,lastname' or
'firstname,lastname': firstname must be at least one character and cannot
contain spaces; middleinitial, if present, is one character; lastname is at
least two characters and can contain spaces.

Content guidelines: List all people associated with this study.
*******************************************************************************/
const char* CSTRING_Series_contributor = "!Series_contributor";
static const std::string STRING_Series_contributor(CSTRING_Series_contributor);
//std::vector<std::string> series_contributor;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: dose, time, tissue, strain, gender, cell line,
development stage, age, agent, cell type, infection, isolate, metabolism, shock,
stress, temperature, specimen, disease state, protocol, growth protocol,
genotype/variation, species, individual, or other

Content guidelines: Indicate the variable type(s) investigated in this study,
e.g.,
!Series_variable_1 = age
!Series_variable_2 = age
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
const char* CSTRING_Series_variable_ = "!Series_variable_";
static const std::string STRING_Series_variable_(CSTRING_Series_variable_);
//std::vector<std::string> series_variable;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: any

Content guidelines: Describe each variable, e.g.,
!Series_variable_description_1 = 2 months
!Series_variable_description_2 = 12 months
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
const char* CSTRING_Series_variable_description_ = "!Series_variable_description_";
static const std::string STRING_Series_variable_description_(CSTRING_Series_variable_description_);
//std::vector<std::string> series_variable_description;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value a valid reference to a ^SAMPLE
identifier, or all

Content guidelines: List which Samples belong to each group, e.g.,
!Series_variable_sample_list_1 = samA, samB
!Series_variable_sample_list_2 = samC, samD
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
const char* CSTRING_Series_variable_sample_list_ = "!Series_variable_sample_list_";
static const std::string STRING_Series_variable_sample_list_(CSTRING_Series_variable_sample_list_);
//std::vector<std::string> series_variable_sample_list;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: biological replicate, technical replicate -
extract, or technical replicate - labeled-extract

Content guidelines: Indicate the repeat type(s), e.g.,
!Series_repeats_1 = biological replicate
!Series_repeats_2 = biological replicate
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
const char* CSTRING_Series_repeats_ = "!Series_repeats_";
static const std::string STRING_Series_repeats_(CSTRING_Series_repeats_);
//std::vector<std::string> series_repeats;


/*******************************************************************************
Number of allowed labels: 0 or more

Allowed values and constraints: each value a valid reference to a ^SAMPLE
identifier, or all

Content guidelines:List which Samples belong to each group, e.g.,
!Series_repeats_sample_list_1 = samA, samB
!Series_repeats_sample_list_2 = samC, samD
NOTE - this information is optional and does not appear in Series records or
downloads, but will be used to assemble corresponding GEO DataSet records.
*******************************************************************************/
const char* CSTRING_Series_repeats_sample_list_ = "!Series_repeats_sample_list_";
static const std::string STRING_Series_repeats_sample_list_(CSTRING_Series_repeats_sample_list_);
//std::vector<std::string> series_repeats_sample_list;


/*******************************************************************************
Number of allowed labels: 1 or more

Allowed values and constraints: valid Sample identifiers

Content guidelines: Reference the Samples that make up this experiment.
Reference the Sample accession numbers (GSMxxx) if the Samples already exists in
GEO, or reference the ^Sample identifiers if they are being submitted in the
same file.
*******************************************************************************/
const char* CSTRING_Series_sample_id = "!Series_sample_id";
static const std::string STRING_Series_sample_id(CSTRING_Series_sample_id);
//std::vector<std::string> series_sample_id;


/*******************************************************************************
Number of allowed labels: 0 or 1

Allowed values and constraints: a valid Series accession number (GSExxx)

Content guidelines: Only use for performing updates to existing GEO records.
*******************************************************************************/
const char* CSTRING_Series_geo_accession = "!Series_geo_accession";
static const std::string STRING_Series_geo_accession(CSTRING_Series_geo_accession);
//std::string series_geo_accession;


const char *GeoSoftKeys[58] = {
  CSTRING_PLATFORM,
  CSTRING_Platform_title,
  CSTRING_Platform_distribution,
  CSTRING_Platform_technology,
  CSTRING_Platform_organism,
  CSTRING_Platform_manufacturer,
  CSTRING_Platform_manufacture_protocol,
  CSTRING_Platform_catalog_number,
  CSTRING_Platform_web_link,
  CSTRING_Platform_support,
  CSTRING_Platform_coating,
  CSTRING_Platform_description,
  CSTRING_Platform_contributor,
  CSTRING_Platform_pubmed_id,
  CSTRING_Platform_geo_accession,
  CSTRING_Platform_table_begin,
  CSTRING_Platform_table_end,
  CSTRING_SAMPLE,
  CSTRING_Sample_title,
  CSTRING_Sample_supplementary_file,
  CSTRING_Sample_table,
  CSTRING_Sample_source_name_ch,
  CSTRING_Sample_organism_ch,
  CSTRING_Sample_characteristics_ch,
  CSTRING_Sample_biomaterial_provider_ch,
  CSTRING_Sample_treatment_protocol_ch,
  CSTRING_Sample_growth_protocol_ch,
  CSTRING_Sample_molecule_ch,
  CSTRING_Sample_extract_protocol_ch,
  CSTRING_Sample_label_ch,
  CSTRING_Sample_label_protocol_ch,
  CSTRING_Sample_hyb_protocol,
  CSTRING_Sample_scan_protocol,
  CSTRING_Sample_data_processing,
  CSTRING_Sample_description,
  CSTRING_Sample_platform_id,
  CSTRING_Sample_geo_accession,
  CSTRING_Sample_anchor,
  CSTRING_Sample_type,
  CSTRING_Sample_tag_count,
  CSTRING_Sample_tag_length,
  CSTRING_Sample_table_begin,
  CSTRING_Sample_table_end,
  CSTRING_SERIES,
  CSTRING_Series_title,
  CSTRING_Series_summary,
  CSTRING_Series_overall_design,
  CSTRING_Series_pubmed_id,
  CSTRING_Series_web_link,
  CSTRING_Series_contributor,
  CSTRING_Series_variable_,
  CSTRING_Series_variable_description_,
  CSTRING_Series_variable_sample_list_,
  CSTRING_Series_repeats_,
  CSTRING_Series_repeats_sample_list_,
  CSTRING_Series_sample_id,
  CSTRING_Series_geo_accession,
  NULL
};


struct GeoSoftChannel{
  //Number of allowed labels: 1 per channel
  std::string sample_source_name;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_organism;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::pair<std::string, std::string> > sample_characteristics;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_biomaterial_provider;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_treatment_protocol;

  //Number of allowed labels: 0 or more per channel
  std::vector<std::string> sample_growth_protocol;

  //Number of allowed labels: 1 per channel
  std::string sample_molecule;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_extract_protocol;

  //Number of allowed labels: 1 per channel
  std::string sample_label;

  //Number of allowed labels: 1 or more per channel
  std::vector<std::string> sample_label_protocol;
};


struct GeoSoft{
  std::string platform;
  std::string platform_title;
  enum platform_distribution_enum platform_distribution;
  enum platform_technology_enum platform_technology;
  std::vector<std::string> platform_organism;
  std::string platform_manufacturer;
  std::vector<std::string> platform_manufacture_protocol;
  std::vector<std::string> platform_catalog_number;
  std::vector<std::string> platform_web_link;
  std::string platform_support;
  std::string platform_coating;
  std::string platform_description;
  std::vector<std::string> platform_contributor;
  std::vector<int> platform_pubmed_id;
  std::string platform_geo_accession;
  std::vector<std::string> platform_table_column_titles;
  std::vector<std::vector<std::string> > platform_table_matrix;
  std::string sample;
  std::string sample_title;
  std::vector<std::string> sample_supplementary_file;
  std::string sample_table;

  std::vector<struct GeoSoftChannel> channel;

  std::vector<std::string> sample_hyb_protocol;
  std::vector<std::string> sample_scan_protocol;
  std::vector<std::string> sample_data_processing;
  std::vector<std::string> sample_description;
  std::string sample_platform_id;
  std::string sample_geo_accession;
  std::string sample_anchor;
  std::string sample_type;
  int sample_tag_count;
  int sample_tag_length;
  std::vector<std::string> sample_table_columns;
  std::vector<std::vector<std::string> > sample_table_matrix;
  std::string series;
  std::string series_title;
  std::vector<std::string> series_summary;
  std::string series_overall_design;
  std::vector<std::string> series_pubmed_id;
  std::vector<std::string> series_web_link;
  std::vector<std::string> series_contributor;
  std::vector<std::string> series_variable;
  std::vector<std::string> series_variable_description;
  std::vector<std::string> series_variable_sample_list;
  std::vector<std::string> series_repeats;
  std::vector<std::string> series_repeats_sample_list;
  std::vector<std::string> series_sample_id;
  std::string series_geo_accession;
};

int loadGeoSoftFile(const char *fp, struct GeoSoft **contents);

#endif
